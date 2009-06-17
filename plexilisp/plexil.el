;;; Copyright (c) 2006-2009, Universities Space Research Association (USRA).
;;;  All rights reserved.
;;;
;;; Redistribution and use in source and binary forms, with or without
;;; modification, are permitted provided that the following conditions are met:
;;;     * Redistributions of source code must retain the above copyright
;;;       notice, this list of conditions and the following disclaimer.
;;;     * Redistributions in binary form must reproduce the above copyright
;;;       notice, this list of conditions and the following disclaimer in the
;;;       documentation and/or other materials provided with the distribution.
;;;     * Neither the name of the Universities Space Research Association nor the
;;;       names of its contributors may be used to endorse or promote products
;;;       derived from this software without specific prior written permission.
;;;
;;; THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
;;; WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
;;; MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;;; DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
;;; INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
;;; BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
;;; OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
;;; ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
;;; TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
;;; USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


;;; PlexiLisp -- An Emacs Lisp based syntax and XML generator for PLEXIL
;;; plans and simulation scripts.  Described in full on its wiki:
;;;   http://universalexec.wiki.sourceforge.net/PlexiLisp

;;; Basic instructions:
;;;    Write a plan or script in PlexiLisp
;;;    In that buffer, type 
;;;      M-x plexil
;;;    If you have nXML mode, the XML can be schema-validated instantly.

;;; Gnu Emacs version 22.x or higher is recommended, though the
;;; essential feature of Plexilisp have worked in earlier versions
;;; (20.4.x, 21.x), and on Emacs variants including XEmacs and Aquamacs.

;;; Types mentioned in this code:
;;;   xml         = an ADT for XML (definition below)
;;;   xml-string  = string            (a well-formed XML expression)
;;;   list(X)     = list of type X
;;;   list        = heterogenous list
;;;   bool        = Lisp boolean      (nil false, non-nil true)
;;;   plexil-bool = PLEXIL boolean    (0 false, 1 true)
;;;   any         = any type
;;;   a,b,c,...   = type variable
;;;   a * b       = product           (function of N args)
;;;   a + b       = sum               (either type a or b)
;;;   a -> b      = function from a to b
;;;   opt(X)      = X + nil
;;;   ...the rest should be intuitive

(defconst *plexil-home* (format "%s" (getenv "PLEXIL_HOME")))
(defconst *plexilisp-location* (format "%s/plexilisp" *plexil-home*))
(defconst *bin-location* (format "%s/bin" *plexil-home*))

(defconst *plexilisp-version* "3.1")

(defvar *node-id-number*) ; supports repeatable, unique node ID's
(defconst *plexil-script-prefix* "ps-")
(defconst *plexil-plan-prefix* "pl-")

;;; These support the 'Action' macro.  They are lists of names (symbols)
;;; of Plexilisp constructs, automatically generated when this file is
;;; evaluated.
(defvar *node-types*) 
(setq *node-types* nil)
(defvar *node-body-types*)
(setq *node-body-types* nil)
(defun node-type? (x) (member x *node-types*))
(defun node-body-type? (x) (member x *node-body-types*))


;;; User interface

(defun plexil ()
  "Creates XML file from current buffer containing PlexiLisp plan or script."
  (interactive)
  (if (null *plexilisp-location*)
      (message (concat "The PLEXILISP_HOME environment variable is undefined. "
                       "It must be set to the pathname of your 'plexilisp' "
                       "directory. Then restart Emacs."))
    (let* ((max-lisp-eval-depth 1000)   ; support huge plans!
           (marker (set-marker (make-marker) 1 (current-buffer)))
           (buffer-content (read marker))
           (type (and (consp buffer-content) (car buffer-content))))
      (setq *node-id-number* 0)      
      (cond ((member type '(plexil-plan PlexilPlan))
             (apply  #'process-plan-buffer
                     (list
                      ".epx" ".plx"
                      (eval
                       (translate-symbols
                        buffer-content
                        (lambda (s)     ;  symbol -> symbol
                          (intern
                           (concat *plexil-plan-prefix* (symbol-name s)))))))))
            ((member type '(plexil-script PlexilScript))
             (apply  #'process-script-buffer
                  (list
                   ".plx"
                   (eval
                    (translate-symbols
                     buffer-content
                     (lambda (s)        ;  symbol -> symbol
                       (intern
                        (concat *plexil-script-prefix* (symbol-name s)))))))))
            (t (message
                "Sorry, this buffer does not contain valid Plexilisp code."))))))


;;; Buffer creation

(defun process-plan-buffer (extended-extension core-extension xml) ; -> ()
  ;; Warning: crude and non-robust! 
  ;; Should be refactored with process-script-buffer
  ;;
  ;; First, save Plexilisp file
  (save-buffer (current-buffer))
  ;; Next, open or create corresponding .epx buffer
  (let ((buffer (find-file
                 (concat (file-name-sans-extension (buffer-name))
                         extended-extension))))
    ;; Wipe out any existing content
    (erase-buffer)
    ;; Write the XML, with preambles
    (insert "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
    (insert (format "<!-- Generated by PlexiLisp -->\n"))
    (insert (xml->string xml))
    (insert "\n")
    ;; Save so that xmlformat can process the file
    (save-buffer buffer)
    (let ((xmlformat (format "%s/xmlformat.pl" *plexilisp-location*))
          (xmlconfig (format "%s/xmlformat.conf" *plexilisp-location*))
          (eplexil (format "%s/eplexil" *bin-location*)))
      ;; Call xmlformat on the output file
      (call-process xmlformat nil buffer nil "-f" xmlconfig "-i" "-b" ".last"
                    (buffer-file-name buffer))
      ;; Open the result of xmlformat (the .epx file)
      (revert-buffer buffer t t)
      ;; Compile the output file into Core Plexil
      (call-process eplexil nil buffer nil (buffer-file-name buffer))
      ;; Open the Core Plexil file
      (let ((plx-filename (concat (file-name-sans-extension (buffer-name))
                                  core-extension)))
        (if (get-buffer plx-filename) (kill-buffer plx-filename))
        (find-file plx-filename)
        ;; Position cursor at start of file.
        (goto-char 0)))))

(defun process-script-buffer (extension xml) ; -> ()
  ;; First, save file
  (save-buffer (current-buffer))
  ;; Next, open or create corresponding .plx buffer
  (let ((buffer (find-file
                 (concat (file-name-sans-extension (buffer-name)) extension))))
    ;; Wipe out any existing content
    (erase-buffer)
    ;; Write the compiled XML, with preambles
    (insert "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
    (insert (format "<!-- Generated by PlexiLisp -->\n"))
    (insert (xml->string xml))
    (insert "\n")
    ;; Save so that xmlformat can process the file
    (save-buffer buffer)
    ;; Call xmlformat on the output file
    (let ((xmlformat (format "%s/xmlformat.pl" *plexilisp-location*))
          (xmlconfig (format "%s/xmlformat.conf" *plexilisp-location*)))
      (call-process xmlformat nil buffer nil "-f" xmlconfig "-i" "-b" ".last"
                    (buffer-file-name buffer)))
    ;; Open the result of xmlformat
    (revert-buffer buffer t t))
  ;; Position cursor at start of file.
  (goto-char 0))


;;; Cheap Namespace

;;; Emacs Lisp does not have namespaces (packages), therefore, to avoid
;;; polluting the global namespace, most functions in this file have
;;; special prefixes:
;;;     pl-     PLEXIL constructs
;;;     ps-     PLEXIL script constructs
;;;     plexil- auxiliary functions
;;; The appropriate prefix is added to all symbols in user input.  This
;;; also prevents arbitrary Lisp calls within the input, which is desirable.

(defun translate-symbols (x f)       ; list * (symbol -> symbol) -> list
  (cond ((null x) x)
        ;; User doesn't need to quote boolean literals, nor do they need
        ;; a namespace prefix.
        ((memq x '(true false unknown)) `(quote ,x))
        ((symbolp x) (funcall f x))
        ((atom x) x)
        ((consp x) (cons (translate-symbols (car x) f)
                         (translate-symbols (cdr x) f)))))


;;; Document Generation

(defconst *assemble-doc*
  ;; To generate the Wiki reference manual, set this to t, evaluate the
  ;; buffer, and call M-x generate-plexil-doc
  nil)

(defvar *plexilisp-reference* nil)
(setq *plexilisp-reference* nil)

(defun generate-plexil-doc ()
  (interactive)
  (let ((max-specpdl-size 5000))
    (when *assemble-doc*
      (let ((buffer (find-file-other-window "plexilisp-reference.out")))
        (erase-buffer)
        (mapc #'insert-plexilisp-entry (reverse *plexilisp-reference*))
        (insert "\n")
        (save-buffer)
        (goto-char 0)))))

(defun insert-plexilisp-entry (e)  ; docitem -> ()
  (cond ((plexil-doc? e)
         (insert "----\n")
         (insert "[[code]]\n")
         (mapc
          (lambda (name)
            (insert (format "(%s %s)\n"
                            name
                            (plexil-format-arglist (plexil-doc-arglist e)))))
          (plexil-doc-names e))
         (insert "[[code]]\n")
         (insert "\n")         
         (let ((desc (plexil-doc-desc e)))
           (if (stringp desc)
               (insert desc)
             (insert (apply #'concat desc))))
         (insert "\n\n"))
        ((plexil-heading? e)
         (insert (format "%s\n\n%s\n\n"
                         (plexil-heading-title e)
                         (plexil-heading-text e))))
        (t (error "Invalid argument to insert-plexilisp-entry: %s" e))))


(defun plexil-format-arglist (x) ; list(string) -> string
  (let ((result ""))
    (while (not (null x))
      (cond ((equal (car x) "&rest")
             (setq result (concat result (plexil-format-&rest (cadr x))))
             (setq x (cddr x)))
            ((equal (car x) "&optional")
             (setq result (concat result (plexil-format-&optional (cdr x))))
             (setq x nil))
            (t
             (setq result (concat result (car x)))
             (if (cdr x) (setq result (concat result " ")))
             (setq x (cdr x)))))
    ;; Fix formatting problem in empty argument lists.
    (if (equal "" result) "``" result)))

(defun plexil-format-&rest (name) ; string -> string
  ;; Assumes name is a plural noun that can be made singular by removing
  ;; the final 's'.
  (format "[ %s ] ..." (substring name 0 (1- (length name)))))

(defun plexil-format-&optional (names) ; list(string) -> string
  (let ((result ""))
    (while (not (null names))
      (setq result (concat result (format "[ %s ] " (car names))))
      (setq names (cdr names)))
    result))
    
;;; ADT for a documenation entry

(defun plexil-doc (names arglist doc)
  ;; list(symbol) * list(symbol) * string -> plexil-doc
  (list '*plexil-doc* names arglist doc))

(defun plexil-doc? (x) ; any -> bool
  (and (consp x)
       (eq '*plexil-doc* (car x))))

(defun plexil-doc-names (x) (second x))
(defun plexil-doc-arglist (x) (third x))
(defun plexil-doc-desc (x) (fourth x))

(defun plexil-heading (title text) ; string * string -> plexil-heading
  (list '*plexil-heading* title text))

(defun plexil-heading? (x) ; any -> bool
  (and (consp x)
       (eq '*plexil-heading* (car x))))

(defun plexil-heading-title (x) (second x))
(defun plexil-heading-text (x) (third x))

(defun insert-plexil-heading (title &rest text)
  (if *assemble-doc*
      (push (plexil-heading title (apply #'concat text)) *plexilisp-reference*)))

;;; Not needed, but duplicated here to obviate a spurious compiler warning.
(defun plexil-qualify (a b)
  (intern (concat (symbol-name a) "-" (symbol-name b))))

;;; Plexilisp defining form

(defmacro pdefine (namespace names arglist indent type doc body)
  ;; symbol * list(symbol) * list(symbol) * any * opt(symbol) * string * any -> ()
  ;;
  ;; Allows 2-4 differently named definitions of the same function, and
  ;; provides automatic generation of the Plexilisp reference manual.
  ;;
  ;; This function is needed only for macro expansion.
  (defun plexil-qualify (a b)
    (intern (concat (symbol-name a) "-" (symbol-name b))))
  `(progn
     (mapc (lambda (name)
             (put name 'plexilisp-indent-function ',indent)
             (cond
              ((eq ',type 'node)
               (push (plexil-qualify ',namespace name) *node-types*))
              ((eq ',type 'node-body)
               (push (plexil-qualify ',namespace name) *node-body-types*))))
           ',names)
     ;; ! This is a hack, but I haven't found a way to generalize it.
     ;; to any number of names.
     (defun ,(plexil-qualify namespace (car names)) ,arglist ,body)
     (defun ,(plexil-qualify namespace (cadr names)) ,arglist ,body)
     (when (> (length ',names) 2) 
       (defun ,(plexil-qualify namespace (car (cddr names))) ,arglist ,body))
     (when (> (length ',names) 3) 
       (defun ,(plexil-qualify namespace (cadr (cddr names))) ,arglist ,body))
     (if *assemble-doc*
         (push (plexil-doc ',names ',(mapcar #'symbol-name arglist) ',doc)
               *plexilisp-reference*))))

;;;  This is a macro version of the above.

(defmacro pdefine-syntax (namespace names arglist indent type doc body)
  ;; symbol * list(symbol) * list(symbol) * any * opt(symbol) * string * any -> ()
  ;;
  ;; Allows 2-4 differently named definitions of the same macro, and
  ;; provides automatic generation of the Plexilisp reference manual.
  ;;
  ;; This function is needed only for macro expansion.
  (defun plexil-qualify (a b)
    (intern (concat (symbol-name a) "-" (symbol-name b))))
  `(progn
     (mapc (lambda (name)
             (put name 'plexilisp-indent-function ',indent)
             (cond
              ((eq ',type 'node)
               (push (plexil-qualify ',namespace name) *node-types*))
              ((eq ',type 'node-body)
               (push (plexil-qualify ',namespace name) *node-body-types*))))
           ',names)
     ;; ! This is a hack, but I haven't found a way to generalize it.
     ;; to any number of names.
     (defmacro ,(plexil-qualify namespace (car names)) ,arglist ,body)
     (defmacro ,(plexil-qualify namespace (cadr names)) ,arglist ,body)
     (when (> (length ',names) 2) 
       (defmacro ,(plexil-qualify namespace (car (cddr names))) ,arglist ,body))
     (when (> (length ',names) 3) 
       (defmacro ,(plexil-qualify namespace (cadr (cddr names))) ,arglist ,body))
     (if *assemble-doc*
         (push (plexil-doc ',names ',(mapcar #'symbol-name arglist) ',doc)
               *plexilisp-reference*))))


;;; ----------------------- Plexilisp Language ----------------------------------

(insert-plexil-heading
 "= Plexilisp Reference Manual = "
 (format
  "//This wiki was automatically generated from {{plexil.el}} on %s \n"
  (current-time-string))
 (format "for version %s.  Do not edit!//\n\n" *plexilisp-version*)
 "This is a complete reference for the Plexilisp language. "
 "It assumes a basic understanding of the PLEXIL language. "
 "Each construct in Plexilisp has at least two aliases "
 "(e.g. {{CommandNode}} and {{command-node}}).  You may use whichever you prefer, "
 "or mix and match them.\n\n"
 "[[toc]]")

(insert-plexil-heading
 "== PLEXIL Plan =="
 "This section describes the forms (constructs) that comprise PLEXIL plans.")
                        
(pdefine pl (PlexilPlan plexil-plan) (form &rest forms) 0 nil ; list(xml) -> xml
  ("The top level form for a plan.  "
   "A Plexilisp file must contain exactly one of these, and nothing else.  "
   "A {{PlexilPlan}} form must contain forms in the following order. "
   "The first (optional) can be a {{GlobalDeclarations}}. "
   "The second (required) is the plan's root node. "
   "Additional forms can only be {{Comment}}'s.")
  (xml "PlexilPlan" (cons form forms)))

(insert-plexil-heading
 "=== Global Declarations ==="
 "")

(pdefine pl (GlobalDeclarations global-declarations Declarations declarations)
            (&rest decls) 0 nil
  ;; list(xml) -> xml
  "The plan's global declarations."
  (xml "GlobalDeclarations" decls))

(pdefine pl (LibraryNodeDeclaration library-node-declaration)
         (name &rest interface-declarations) 0 nil
  ;; string * list(xml) -> xml
  ("Declare a library node (call).  Following the name may be any number of "
   "interface declarations, which are either {{In}} or {{InOut}} forms.")
  (xml "LibraryNodeDeclaration"
       (list (xml "Name" name)
             (xml "Interface" interface-declarations))))

(pdefine pl (StateDeclaration state-declaration)
            (name &rest returns-then-parameters) 0 nil
  ;; string * list(xml) -> xml
  ("Declare a state (lookup).  Following the name should be zero or more Return "
   "forms, then zero or more Parameter forms.  They cannot be intermixed.")
  (plexil-external-call-declaration "StateDeclaration" name
                                    returns-then-parameters))

(pdefine pl (FunctionDeclaration function-declaration)
            (name &rest returns-then-parameters) 0 nil
  ;; string * list(xml) -> xml
  ("Declare a function call.  Following the name should be zero or more Return "
   "forms, then zero or more Parameter forms.  They cannot be intermixed.")
  (plexil-external-call-declaration "FunctionDeclaration" name
                                    returns-then-parameters))

(pdefine pl (CommandDeclaration command-declaration)
            (name &rest returns-then-parameters-then-resource-list) 0 nil
  ;; string * list(xml) -> xml
  ("Declare a command.  Following the name should be zero or more Return "
   "forms, then zero or more Parameter forms, then an optional ResourceList "
   "form.  They cannot be intermixed.")
  (plexil-external-call-declaration "CommandDeclaration" name
                                    returns-then-parameters-then-resource-list))

(pdefine pl (Return return) (type) 1 nil
  ;; string -> xml
  ("Specify a return type.  Argument must be one of 'string', 'integer', "
   "'array', 'boolean', 'real' (case doesn't matter).")
  (plexil-type-declaration "Return" type))

(pdefine pl (Parameter parameter) (type) 1 nil
  ;; string -> xml
  ("Specify a parameter type.  Argument must be one of 'string', 'integer', "
   "'array', 'boolean', 'real' (case doesn't matter).")
  (plexil-type-declaration "Parameter" type))

(defun plexil-type-declaration (name type)
  ;; string * string -> xml
  "Constructs a Return or Parameter element."
  (xml name nil (list (cons "Type" (capitalize type)))))

(defun plexil-external-call-declaration (type name args)
  ;; string * string * list(xml) -> xml
  "Declaration of command, function call, or state"
  (xml type
       (cons (xml "Name" name) args)))

(insert-plexil-heading
 "=== Nodes and Node Types ==="
 "These are the forms for defining PLEXIL nodes. "
 "It takes at least two to fully define a node. "
 "An outer form declares the node. "
 "(e.g. {{CommandNode}}), and an inner form defines the action "
 "(e.g. {{(command ...)}}).  These forms must be compatible "
 "(more specifics in each entry below)")

(pdefine pl (ListNode list-node) (&optional name &rest node-clauses) 1 node
  ;; string * list(xml) -> xml
  "Defines a List Node.  Must contain a {{List}} form." 
  (plexil-node name node-clauses "NodeList"))

(pdefine pl (List list) (&rest nodes) 0 node-body       ; list(xml) -> xml
  "Required inside a {{ListNode}}, this form wraps its list of nodes."
  (xml "NodeBody"
       (xml "NodeList" nodes)))

(pdefine pl (CommandNode command-node) (&optional name &rest clauses) 1 node
  ;; string * list(xml) -> xml
  ("Defines a Command Node.  Must contain either a {{Command}} or "
   "{{CommandWithReturn}} form.")
  (plexil-node name clauses "Command"))

(pdefine pl (Command command) (command-name &rest args) 1 node-body
  ;; (string + xml) * list(xml) -> xml         
  ("Required inside a {{CommandNode}}, this form calls the specified command. "
   "command-name may be any string "
   "expression (literal, variable, concatenation, or lookup). "
   "If resources are specified, they must follow the command name.")
  (let ((resources (plexil-extract-resources-from-args args)))
    (if resources
        (plexil-command command-name nil resources (cdr args))
      (plexil-command command-name nil nil args))))

(pdefine pl (CommandWithReturn command-with-return) (command-name var &rest args)
         1 node-body
  ;; (string + xml) * list(xml) -> xml
  ("This is just like {{Command}} above, but a value returned from the command "
   "is assigned to the given variable, which must be declared in this node or "
   "one of its ancestors.")
  (let ((resources (plexil-extract-resources-from-args args)))
    (if resources
        (plexil-command command-name var resources (cdr args))
      (plexil-command command-name var nil args))))

(defun plexil-extract-resources-from-args (args) ; list(xml) -> opt(xml)
  (if (> (length args) 0)
      (xml-assoc-element "ResourceList" (first args))))

(pdefine pl (UpdateNode update-node) (&optional name &rest clauses) 1 node
  ;; string * list(xml) -> xml
  "Defines an Update Node.  Must contain an {{Update}} form."
  (plexil-node name clauses "Update"))

(pdefine pl (Update update) (&rest pairs) 0 node-body     ; list(xml) -> xml
  ("Required inside an {{UpdateNode}}, this form defines the plan update."
   "It must contain one or more {{Pair}} forms.")
  (plexil-node-body (xml "Update" pairs)))

(pdefine pl (Pair pair) (name value) 2 nil            ; string * any -> xml
  ("Required inside an {{Update}}, this form defines a name/value pair."
   "The {{name}} must be a string and the {{value}} may be any PLEXIL type.")
  (xml "Pair" (list (xml "Name" name) (infer-type value))))


(pdefine pl (FunctionCallNode function-call-node) (&optional name &rest clauses)
         1 node
  ;; string * list(xml) -> xml
  "Defines a Function Call Node.  Must contain a {{FunctionCall}} form."
  (plexil-node name clauses "FunctionCall"))

(pdefine pl (FunctionCall function-call) (function-name &rest args) 0 node-body
  ;; string * list(xml) -> xml
  ("Valid inside a {{FunctionCallNode}}, this form calls the named function "
   "with the given arguments.")
  (plexil-node-body
   (xml "FunctionCall"
        (append (list (xml "FunctionName" function-name))
                (if args (list (xml "Arguments" (mapcar #'infer-type args))))))))

(pdefine pl (FunctionCallWithResult function-call-with-result)
         (var function-name &rest args) 0 node-body
  ;; xml * string * list(xml) -> xml
  ("Valid inside a {{FunctionCallNode}}, this form calls the named function "
   "with the given arguments, and assigns the return value to //var//, which "
   "must be declared in this node or one of its ancestors.")
  (plexil-node-body
   (xml "FunctionCall"
        (append (list var)
                (list (xml "FunctionName" function-name))
                (if args (list (xml "Arguments" (mapcar #'infer-type args))))))))

(pdefine pl (AssignmentNode assignment-node) (&optional name &rest clauses) 1 node
  ;; string * list(xml) -> xml
  "Defines an Assignment Node.  Must contain an {{Assignment}} form."
  (plexil-node name clauses "Assignment"))

(pdefine pl (Assignment assignment) (var val) 2 node-body
  ;; xml * (xml + int + bool + string) -> xml
  ("Required inside an {{AssignmentNode}}, this form assigns a value (any PLEXIL "
   "type) to a variable that must be declared in this node or one of its "
   "ancestors.")
  (let ((vall (infer-type val)))
    (xml "NodeBody"
         (xml "Assignment"
              (list var
                    (cond
                     ((and (number? var) (number? vall))
                      (xml "NumericRHS" vall))
                     ((and (string? var) (string? vall))
                      (xml "StringRHS" vall))
                     ((and (boolean? var) (boolean? vall))
                      (xml "BooleanRHS" vall))
                     ((and (time? var) (time? vall))
                      (xml "TimeRHS" vall))
                     (t
                      (error "Illegal args in assignment: %s %s" var val))))))))


(defun plexil-command (command var resources args)
  ;; (string + xml) * opt(xml) * opt(xml) * list(xml) -> xml
  (let ((name (plexil-normalize-string-expression command)))
    (plexil-node-body
     (xml "Command"
          (append (if resources (list (xml "ResourceList" resources)))
                  (if var (list var))
                  (list (xml "Name" name))
                  (if args (list (xml "Arguments"
                                      (mapcar #'infer-type args)))))))))

(pdefine pl (LibraryCallNode library-call-node) 
         (&optional name &rest node-clauses) 1 node
  ;; string * list(xml) -> xml
  "A Library Call Node.  Must contain exactly one {{call}} form."
  (plexil-node name node-clauses "LibraryNodeCall"))

(pdefine pl (Call call) (nodeid &rest aliases) 1 node-body
  ;; string * list(xml) -> xml
  "A call to a library node."
  (xml "NodeBody"
       (xml "LibraryNodeCall"
            (cons (xml "NodeId" nodeid) aliases))))

(pdefine pl (Alias alias) (parameter value) 2 nil
  ;; ncName * (xml + literal) -> xml
  ("In a library node call, this pairs a parameter of the node with "
   "a value.  The parameter is an ncName, and the value must be either "
   "a literal or declared variable.")
  (xml "Alias" (list (xml "NodeParameter" parameter) (infer-type value))))

(pdefine pl (EmptyNode empty-node) (&optional name &rest clauses) 1 node
  ;; opt(string + xml) * list(xml) -> xml
  "An Empty Node."
  (plexil-node name clauses "Empty"))

(defun plexil-node-body (body)              ; xml -> xml
  (xml "NodeBody" body))

(defun plexil-node (name-or-first-clause clauses type)
  ;; (string + xml) * list(xml) * string -> xml
  (let ((the-name (if (stringp name-or-first-clause)
                      name-or-first-clause
                    (plexil-unique-node-id "plexilisp_Name")))
        (the-clauses (cond
                      ((stringp name-or-first-clause) clauses)
                      ((null name-or-first-clause) nil)
                      (t (cons name-or-first-clause clauses)))))
    (xml "Node" (cons (xml "NodeId" the-name) the-clauses)
         `(("NodeType" . ,type)))))

(insert-plexil-heading "=== Variable Declaration ===")

;;; Stub that only obviates a compiler warning
(defun pl-variables (&rest decls))

(pdefine pl (VariableDeclarations variable-declarations Variables variables)
         (&rest decls) 0 nil
  ;; list(xml) -> xml
  ("The node's variable declarations.  Must contain one or more of the "
   "declaration forms that follow.")
  (xml "VariableDeclarations" decls))

(pdefine pl (DeclareVariables declare-variables Let let)
         (&rest decls) 0 nil
  ;; list(xml) -> xml
  "More synonyms for the VariableDeclarations form."
  (xml "VariableDeclarations" decls))

(pdefine pl (Integer integer) (name &optional val) 2 nil ; string * opt(int) -> xml
  "Declare an integer variable, with optional initial value."
  (xml "DeclareVariable"
       (cons (xml "Name" name)
             (cons (xml "Type" "Integer")
                   (if val
                       (list (xml "InitialValue" (xml "IntegerValue" val))))))))

(pdefine pl (Real real) (name &optional val) 2 nil    ; string * opt(real) -> xml
  "Declare a real variable, with optional initial value." 
  (xml "DeclareVariable"
       (cons (xml "Name" name)
             (cons (xml "Type" "Real")
                   (if val
                       (list (xml "InitialValue" (xml "RealValue" val))))))))

(pdefine pl (Boolean boolean) (name &optional val) 2 nil ; string * opt(bool) -> xml
  "Declare a boolean variable, with optional initial value." 
  (xml "DeclareVariable"
       (cons (xml "Name" name)
             (cons (xml "Type" "Boolean")
                   (if val
                       (list (xml "InitialValue" (xml "BooleanValue" val))))))))

(pdefine pl (String string) (name &optional val) 2 nil ; string * opt(string) -> xml
  "Declare a string variable, with optional initial value." 
  (xml "DeclareVariable"
       (cons (xml "Name" name)
             (cons (xml "Type" "String")
                   (if val
                       (list (xml "InitialValue" (xml "StringValue" val))))))))

(pdefine pl (IntArray int-array) (name size &rest values) 2 nil
         ;; string * int * list(int + xml) -> xml
  "Declare an integer array with given name, size, and initial values."
  (plexil-array name "Integer" size values))

(pdefine pl (StringArray string-array) (name size &rest values) 2 nil
         ;; string * int * list(string + xml) -> xml
  "Declare a string array with given name, size, and initial values."
  (plexil-array name "String" size values))

(pdefine pl (BooleanArray boolean-array) (name size &rest values) 2 nil
         ;; string * int * list(bool + xml) -> xml
  "Declare a boolean array with given name, size, and initial values."
  (plexil-array name "Boolean" size values))

(pdefine pl (RealArray real-array) (name size &rest values) 2 nil
         ;; string * int * list(real + xml) -> xml
  "Declare a real number array with given name, size, and initial values."
  (plexil-array name "Real" size values))

(defun plexil-array (name type size values)
  ;; string * string * int * list(a) -> xml
  (xml "DeclareArray"
       (cons (xml "Name" name)
             (cons (xml "Type" type)
                   (cons (xml "MaxSize" size)
                         (if values
                             (list (xml "InitialValue"
                                        (mapcar
                                         (lambda (x)
                                           (xml (format "%sValue" type) x))
                                         values)))))))))

;;; Conditions, all: xml -> xml

(insert-plexil-heading "=== Node Conditions ===")

(pdefine pl (Postcondition postcondition) (exp) 0 nil
  ""         
  (xml "PostCondition" (infer-type exp)))

(pdefine pl (PostCondition post-condition) (exp) 0 nil
  ""
  (xml "PostCondition" (infer-type exp)))

(pdefine pl (EndCondition end-condition) (exp) 0 nil
  ""
  (xml "EndCondition" (infer-type exp)))

(pdefine pl (SkipCondition skip-condition) (exp) 0 nil
  ""
  (xml "SkipCondition" (infer-type exp)))

(pdefine pl (Precondition precondition) (exp) 0 nil
  ""
  (xml "PreCondition" (infer-type exp)))

(pdefine pl (PreCondition pre-condition) (exp) 0 nil
  ""
  (xml "PreCondition" (infer-type exp)))

(pdefine pl (RepeatCondition repeat-condition) (exp) 0 nil
  ""
  (xml "RepeatCondition" (infer-type exp)))

(pdefine pl (StartCondition start-condition) (exp) 0 nil
  ""
  (xml "StartCondition" (infer-type exp)))

(pdefine pl (InvariantCondition invariant-condition) (exp) 0 nil
  ""
  (xml "InvariantCondition" (infer-type exp)))

;;; Variable Reference

(insert-plexil-heading
 "=== Variable Reference ==="
 "All variable references must take one of the  following forms, which "
 "specifies the name of the variable as a string.  The variable is assumed "
 "to be legally declared.")

(pdefine pl (BooleanVariable boolvar) (name) 1 nil               ; string -> xml
  ""
  (xml "BooleanVariable" name nil 'boolean))

(pdefine pl (IntegerVariable intvar) (name) 1 nil               ; string -> xml
  ""
  (xml "IntegerVariable" name nil 'integer))

(pdefine pl (RealVariable realvar) (name) 2 nil              ; string -> xml
  ""
  (xml "RealVariable" name nil 'real))

(pdefine pl (StringVariable stringvar) (name) 1 nil            ; string -> xml
  ""
  (xml "StringVariable" name nil 'string))

(pdefine pl (ArrayVariable arrayvar) (name) 1 nil             ; string -> xml
  ""
  (xml "ArrayVariable" name nil 'string))

(pdefine pl (ArrayElement array-element) (name index) 2 nil
         ;; (string + xml) * (int + xml) -> xml
  ("Reference a single array element by index (beginning with 0).  "
   "Name and index can be string and numeric expressions, respectively.")
  (xml "ArrayElement"
       (list (xml "Name" (infer-type name))
             (xml "Index" (infer-type index))) nil 'any))
  
(insert-plexil-heading
 "=== Interface Declaration ==="
 "Plexilisp does not automatically generate any {{Interface}} declarations. "
 "They must be created explicitly with these forms.")

(pdefine pl (Interface interface) (&rest decls) 0 nil     ; list(xml) -> xml
  ("The Node's interface.  This must contain only {{In}} and {{InOut}} forms. "
   "They can be intermixed.")
  (xml "Interface" decls))

(pdefine pl (In in) (&rest vars) 0 nil             ; list(xml) -> xml
  ("Declare input variables.  Your must use the variable declaration "
   "forms defined above.")
  (xml "In" vars))

(pdefine pl (InOut inout) (&rest vars) 1 nil          ; list(xml) -> xml
  ("Declare input/ouput variables.  Your must use the variable declaration "
   "forms defined above.")
  (xml "InOut" vars))

;;; Predicates, all: xml * xml -> xml

(insert-plexil-heading
 "=== Boolean Comparisons ==="
 "These return true or false.")
                       

(pdefine pl (= eq) (x y) 2 nil
  ""
  (pl-comparison "EQ" x y))

(pdefine pl (!= ne) (x y) 2 nil
  ""
  (pl-comparison "NE" x y))

(defun pl-comparison (name x y)
  (let ((xx (infer-type x))
        (yy (infer-type y)))
    (cond ((and (number? xx) (number? yy))
           (xml (concat name "Numeric") (list xx yy) nil 'boolean))
          ((and (string? xx) (string? yy))
           (xml (concat name "String") (list xx yy) nil 'boolean))
          ((and (boolean? xx) (boolean? yy))
           (xml (concat name "Boolean") (list xx yy) nil 'boolean))
          (t (error "Illegal args to %s: %s %s" name x y)))))


;;; Numeric comparisons, all:  xml * xml -> xml

(insert-plexil-heading "The following work with all numeric types.")

(pdefine pl (> gt)  (x y) 2 nil
  ""
  (plexil-numeric-comparison "GT" x y))

(pdefine pl (>= ge) (x y) 2 nil
  ""
  (plexil-numeric-comparison "GE" x y))

(pdefine pl (< lt)  (x y) 2 nil
  ""
  (plexil-numeric-comparison "LT" x y))

(pdefine pl (<= le) (x y) 2 nil
  ""
  (plexil-numeric-comparison "LE" x y))

(defun plexil-numeric-comparison (name x y)
  (let ((xx (infer-type x))
        (yy (infer-type y)))
    (if (and (number? xx) (number? yy))
        (xml name (list xx yy) nil 'boolean)
      (error "Illegal args to %s: %s %s" name x y))))


(insert-plexil-heading
 "=== Logical Connectives ==="
 "These return true, false, or unknown.")

(pdefine pl (Or or) (&rest disjuncts) 0  nil        ; list(xml) -> xml
  "Permits 0 or more disjuncts. {{(Or)}} = {{false}}."
  (let ((evaled (mapcar #'infer-type disjuncts)))
    (xml "OR" evaled nil 'boolean)))

(pdefine pl (And and) (&rest conjuncts) 0 nil       ; list(xml) -> xml
  "Permits 0 or more conjuncts. {{(And)}} = {{true}}."
  (let ((evaled (mapcar #'infer-type conjuncts)))
    (xml "AND" evaled nil 'boolean)))

(pdefine pl (Not not) (x) 1  nil                     ; xml -> xml
  ""
  
  (xml "NOT" (infer-type x) nil 'boolean))

(insert-plexil-heading
 "=== Numeric Operators ==="
 "These should be self-explanatory.  They work with integer or real values.")

(pdefine pl (+ add) (x y) 2 nil
  ""
  (plexil-binary-numop "ADD" x y))

(pdefine pl (- sub) (x y) 2 nil
  ""
  (plexil-binary-numop "SUB" x y))

(pdefine pl (* mul) (x y) 2 nil
  ""
  (plexil-binary-numop "MUL" x y))

(pdefine pl (/ div) (x y) 2 nil
  ""
  (plexil-binary-numop "DIV" x y))

(pdefine pl (Abs abs) (x) 2 nil
  ""
  (plexil-unary-numop "ABS" x))

(pdefine pl (Sqrt sqrt) (x) 2 nil
  ""
  (plexil-unary-numop "SQRT" x))

(defun plexil-unary-numop (name x)
  (let ((xx (infer-type x)))
    (if (number? xx)
        (xml name xx)
      (error "Illegal argument to %s: %s" name x))))

(defun plexil-binary-numop (name x y)
  (let ((xx (infer-type x))
        (yy (infer-type y)))
    (if (and (number? xx) (number? yy))
        (xml name (list xx yy) nil 'number)
      (error "Illegal arguments in %s: %s %s" name x y))))

(insert-plexil-heading
 "=== Lookups ==="
 "In Plexilisp, these all return values of 'any' type, which means they are "
 "compatible with any kind of variable or value for assignments, comparisons, "
 "and operations.  It is up to you to see that the types are compatible.  "
 "Also, the //state// may be specified by any kind of string expression "
 "(literal, variable, concatentation, or even another lookup).")

(pdefine pl (LookupNow lookup-now) (state &rest args) 1 nil
  ;; (string + xml) * list(xml) -> xml
  "Queries for the value of the given state with given arguments."
  (let ((state-name (plexil-normalize-string-expression state)))
    (xml "LookupNow" (append (list (xml "Name" state-name))
                             (if args (list (xml "Arguments"
                                                 (mapcar #'infer-type args)))))
         nil 'any)))

(pdefine pl (LookupOnChange lookup-on-change) (state &rest args) 1 nil
  ;; (string + xml) * list(xml) -> xml
  ("Subscribes for updates of the given state with given arguments."
   "Uses default tolerance value.")
  (let ((state-name (plexil-normalize-string-expression state)))
    (plexil-lookup-on-change state-name args)))

(pdefine pl (LookupOnChangeWithTolerance lookup-on-change-with-tolerance)
  (state tolerance &rest args) 2 nil
  ;; (string + xml) * xml * list(xml) -> xml
  "Like the above, but uses the specified tolerance."
  (let ((state-name (plexil-normalize-string-expression state)))
    (plexil-lookup-on-change state-name args tolerance)))

(defun plexil-lookup-on-change (state-name args &optional tolerance)
  ;; string * list(xml) * opt(xml) -> xml
  (xml "LookupOnChange"
       (append (list (xml "Name" state-name))
               (if tolerance (list (xml "Tolerance" tolerance)))
               (if args (list (xml "Arguments" (mapcar #'infer-type args)))))
       nil 'any))

(defun plexil-normalize-string-expression (x)
  (cond
   ((stringp x) (pl-stringval x))
   ((xml? x) x)
   (t (error "Bad state or command name: %s" x))))

(insert-plexil-heading
 "=== Literals ==="
 "Most of these are not needed, because Plexilisp automatically infers "
 "types of literals.  For example, 5.5 would be a real, 5 would be an integer, "
 "\"foo\" a string, {{true}} and {{false}} a boolean.")

(pdefine pl (IntegerValue intval) (val) 1 nil                ; string -> xml
  ""
  (xml "IntegerValue" val nil 'integer))

(pdefine pl (RealValue realval) (val) 1 nil               ; string -> xml
  ""
  (xml "RealValue" val nil 'real))

(pdefine pl (BooleanValue boolval)  (val) 1 nil
  ;; string | symbol | int -> xml
  ;; valid arguments are 0, 1
  ;; need to test if these will work: true, false, "true", "false"
  ("Valid arguments are 0, 1, and UNKNOWN. "
  "More simply, the symbols {{true}}, {{false}}, and {{unknown}} may be used "
  "instead of this form.")
  (xml "BooleanValue" val nil 'boolean))

(pdefine pl (StringValue stringval) (str &rest strs) 1 nil
  ;; string * list(string) -> xml
  ("Concatenates its arguments into one string.  For long strings, "
   "this makes your file more readable.")
  (xml "StringValue" (concat str (apply #'concat strs)) nil 'string))


;;; Syntactic Sugar

(insert-plexil-heading
 "=== Node State, Outcome, and Failure Type ==="
 "Predicates for querying the state, outcome, and failure type of actions.")

;;; All have type:  string -> xml

(pdefine pl (Finished finished isFinished is-finished) (id) 1 nil
  "Is the given action in state FINISHED?"
  (xml "NodeFinished" id))

(pdefine pl (IterationEnded iteration-ended
             isIterationEnded is-iteration-ended) (id) 1 nil
  "Is the given node in state ITERATION_ENDED?"
  (xml "NodeIterationEnded" id))

(pdefine pl (Executing executing isExecuting is-executing) (id) 1 nil
  "Is the given action in state EXECUTING?"
  (xml "NodeExecuting" id))

(pdefine pl (Waiting waiting isWaiting is-waiting) (id) 1 nil
  "Is the given action in state WAITING?"
  (xml "NodeWaiting" id))

(pdefine pl (Inactive inactive isInactive is-inactive) (id) 1 nil
  "Is the given action in state INACTIVE?"
  (xml "NodeInactive" id))

(pdefine pl (Successful successful isSuccessful is-successful) (id) 1 nil
 "Did the given action finish successfully?"
  (xml "NodeSucceeded" id))

(pdefine pl (IterationSuccessful iteration-successful) (id) 1 nil
 "Did the last iteration of the given action finish successfully?"
  (xml "NodeIterationSucceeded" id))

(pdefine pl (IterationFailed iteration-failed) (id) 1 nil
 "Did the last iteration of the given action fail?"
  (xml "NodeIterationFailed" id))

(pdefine pl (Failed failed isFailed is-failed) (id) 1 nil
  "Did the given action fail?"
  (xml "NodeFailed" id))

(pdefine pl (Skipped skipped isSkipped is-skipped) (id) 1 nil
  "Was the given action skipped?"
  (xml "NodeSkipped" id))

(pdefine pl (InvariantFailed invariant-failed) (id) 1 nil
  "Did the invariant condition of the given action fail?"
  (xml "NodeInvariantFailed" id))

(pdefine pl (PostConditionFailed
             PostconditionFailed
             postcondition-failed
             post-condition-failed) (id) 1 nil
  "Did the postcondition of the given action fail?"
  (xml "NodePostconditionFailed" id))

(pdefine pl (PreConditionFailed
             PreconditionFailed
             precondition-failed
             pre-condition-failed) (id) 1 nil
  "Did the precondition of the given action fail?"
  (xml "NodePreconditionFailed" id))

(pdefine pl (ParentFailed parent-failed) (id) 1 nil
  "Did the parent of the given action fail?"
  (xml "NodeParentFailed" id))

(insert-plexil-heading
 "=== Conditionals and Loops ==="
 "These are high level syntax extensions of PLEXIL (syntactic sugar).  "
 "They expand into node structures. ")


(pdefine pl (If if) (condition then-part &optional else-part) 1 node
  ("If-then-else.  The {{then-part}} and {{else-part}} may be nodes or other "
   "actions.  The {{else-part}} is optional.")
  (xml "If"
       (remove-nil
        (xml "Condition" (infer-type condition))
        (xml "Then" then-part)
        (if else-part (xml "Else" else-part)))))

(pdefine pl (While while) (condition form) 1 node
  "While loop."
  (xml "While"
       (list
        (xml "Condition" (infer-type condition))
        form)))

(pdefine-syntax pl (for For)
                (declaration condition update form &rest forms) 1 node
  ("For Loop.  The declaration should look like a variable declaration. i.e "
   "{{(type name [init])}}, where {{type}} must be either {{integer}} or {{real}} "
   "and the initial value {{init}} is optional (though generally useful).  "
   "{{condition}} is a boolean expression that will terminate the loop when "
   "it is false.  {{update}} is a numeric expression that expresses a new value "
   "for the declared variable.  The one or more actions that follow will be "
   "executed concurrently.")
  ;; list(string,string,number) * xml * xml * xml * list(xml) -> xml
  `(xml "For"
        (list
         (xml "LoopVariable" ,declaration)
         (xml "Condition" ,condition)
         (xml "LoopVariableUpdate" ,update)
         (xml "Actions" (cons ,form (mapcar #'eval ',forms))))))


(pdefine pl (Sequence sequence)
         (&optional name-or-first-form &rest forms) 1 node
  ("Each action starts after the previous succeeds.  "
   "If an action fails, the sequence terminates immediately with failure.")
  (plexil-build-sequence name-or-first-form forms "Sequence"))

(pdefine pl (UncheckedSequence unchecked-sequence) 
         (&optional name-or-first-form &rest forms) 1 node
  "Each action starts after the previous finishes, regardless of success or failure."
  (plexil-build-sequence name-or-first-form forms "UncheckedSequence"))

(pdefine pl (Try try) (&optional name-or-first-form &rest forms) 1 node
  ("Executes actions sequentially, stopping after the an action succeeds.  "
   "Fails if and only if no action succeeds.")
  (plexil-build-sequence name-or-first-form forms "Try"))

(defun plexil-build-sequence (name-or-first-form forms construct)
  ;; (string | xml) * list(xml) * string -> xml
  (let (the-name first-form rest-forms)
    (cond ((stringp name-or-first-form)
           (setq the-name name-or-first-form)
           (setq first-form (car forms))
           (setq rest-forms (cdr forms)))
          ((xml? name-or-first-form)
           (setq first-form name-or-first-form)
           (setq rest-forms forms))
          (t (error "%s must have at least one action!" construct)))
    (xml construct
         (append
          (if the-name (list (xml "NodeId" the-name)))
          (cons first-form rest-forms)))))

(pdefine pl (Concurrently concurrently) (form &rest forms) 0 node
  "Executes forms concurrently.  Just an anonymous List node."
  (pl-list-node (plexil-unique-node-id "plexilisp_Concurrently")
   (apply
    #'pl-list
    (cons form forms))))

(insert-plexil-heading
 "=== Special Purpose Nodes ==="
 "These forms expand into nodes that perform convenient functions.")

(pdefine-syntax pl (Action action) (name &rest forms) 1 node
  ;; string * list(xml) -> xml
  ("Specifies any kind of action.  The specified forms can include any node "
   "clauses (except NodeId, which is given by {{name}}, as well as any number "
   "of actions.  The actions form the body of the generated List Node.")
  (let ((nodes (filter (lambda (x) (node-type? (car x))) forms))
        (node-bodies (filter (lambda (x) (node-body-type? (car x))) forms))
        (clauses (filter (lambda (x)
                           (and (not (node-type? (car x)))
                                (not (node-body-type? (car x)))))
                         forms)))
    (if (not (or nodes node-bodies))
        (append '(pl-empty-node) `(,name) `,clauses)
      (append '(pl-list-node)
              `(,name)
              `,clauses
              (list
               `(apply #'pl-list
                       (append
                        (mapcar #'plexil-eval-node ',nodes)
                        (mapcar #'plexil-eval-node-body ',node-bodies))))))))

(defun plexil-eval-node (node)  ; list -> xml
  (eval node))

(defun plexil-eval-node-body (body)  ; list -> xml
  ;; Crude: could do something data-directed
  (let ((type (car body)))
    (cond
     ((member type '(pl-command
                     pl-Command
                     pl-CommandWithReturn
                     pl-command-with-return))
      (funcall #'pl-command-node (eval body)))
     ((member type '(pl-assignment pl-Assignment))
      (funcall #'pl-assignment-node (eval body)))
     ((member type '(pl-update pl-Update))
      (funcall #'pl-update-node (eval body)))
     ((member type '(pl-library-call pl-LibraryCall))
      (funcall #'pl-library-call-node (eval body)))
     ((member type '(pl-FunctionCall pl-function-call))
      (funcall #'pl-function-call-node (eval body)))
     (t (error "Unknown action: %s" body)))))

(pdefine pl (Nothing nothing) ()  0 node
  "An action that does nothing.  This becomes an anonymous empty node."
  (pl-empty-node))

(pdefine pl (When when) (condition form &rest forms) 1 node
  ;; xml * xml * list(xml) -> xml
  ("Executes actions (concurrently) when condition becomes true.  "
   "This is essentially a //monitor//.")
  (pl-list-node
   (pl-start-condition condition)
   (apply #'pl-list (cons form forms))))

(pdefine pl (Wait wait) (seconds &optional name) 2 node
  ;; real * opt(string) -> xml
  "Waits given number of seconds"
  (let ((nodeid (or name (plexil-unique-node-id "plexilisp_Wait"))))
    (pl-empty-node
     nodeid
     (pl-end-condition (pl-> (pl-lookup-on-change "time")
                             (pl-+ seconds (pl-start-time nodeid)))))))

(pdefine-syntax pl (let Let) (vars form &rest forms) 1 node
  ("Declares variables that are lexically scoped to the enclosing forms, "
   "similar to LET in Lisp.")
  `(pl-list-node
    (apply #'pl-variables (mapcar #'eval ',vars))
    (apply #'pl-list (cons ,form (mapcar #'eval ',forms)))))


(insert-plexil-heading
 "=== Resource Clauses ==="
 "")

(pdefine pl (ResourceList resource-list Resources resources) (&rest resources) 0 nil
  ;; list(xml) -> xml
  "List of Resource specifications."
  (xml "ResourceList" resources))

(pdefine pl (Resource resource) (name priority &rest clauses) 0 nil
  ;; (xml + string) * (xml + int) * list(xml) -> xml
  ("A Resource specification.  Name and priority are required. "
   "The remaining clauses can be {{ResourceUpperBound}} or {{ResourceLowerBound}}")
  (let ((resource-name (plexil-normalize-string-expression name)))
  (xml "Resource"
       (cons (xml "ResourceName" resource-name)
             (cons (xml "ResourcePriority" (infer-type priority))
                   clauses)))))

(pdefine pl (ResourceUpperBound resource-upper-bound) (x) 0 nil
  ;; (xml + real) -> xml
  "A resource upper bound."
  (xml "ResourceUpperBound" (infer-type x)))

(pdefine pl (ResourceLowerBound resource-lower-bound) (x) 0 nil
  ;; (xml + real) -> xml
  "A resource lower bound."
  (xml "ResourceLowerBound" (infer-type x)))
       

(insert-plexil-heading
 "=== Miscellaneous Node Clauses ==="
 "")

(pdefine pl (NodeComment node-comment) (&rest sentences) 1 nil  
  ;; list(string) -> xml
  "A comment for the node. (The sentences will be concatenated.)"
  (xml "Comment" (apply (function concat) sentences)))

(pdefine pl (Permissions permissions) (p) 1 nil             ; string -> xml
  "The node's permissions."
  (xml "Permissions" p))

(pdefine pl (Comment comment) (&rest sentences) 1 nil   ; list(string) -> xml
  ;; XML comments
  ("This creates a comment in the XML, and may occur in any number within "
   "a node.  It is useful for commenting your plan in a way that will "
   "also be reflected in the XML.")
  (xml (apply #'concat sentences) nil nil 'comment))

(insert-plexil-heading
 "=== Miscellaneous Expressions ==="
 "")

(pdefine pl (Concat concat) (&rest exprs) 0 nil
  ;; StringExpression * StringExpression * list(StringExpression) -> xml
  ("A string expression is either a string literal, StringValue expression, "
   "string variable, lookup, or another Concat expression.  The Concat form "
   "takes 0 or more of these and returns a string that is the concatenation "
   "of the evaluated expressions.  Concat() is the empty string.  "
   "Concat(x) = x")
  (xml "Concat"
       (mapcar #'infer-type exprs)
       nil 'string))

(pdefine pl (TimepointValue timepoint-value)
         (nodeid node-state-value timepoint) 1 nil
  ;; string * string * string -> xml
  ("Returns the amount of time since the specified state of the specified node "
   "has either started or ended.  {{node-state-value must be one of INACTIVE, "
   "WAITING, FINISHED, ITERATION_ENDED, EXECUTING, FAILING, FINISHING. "
   "Timepoint must be one of START, END.")
  (xml "NodeTimepointValue"
       (list (xml "NodeId" nodeid)
             (xml "NodeStateValue" node-state-value)
             (xml "Timepoint" timepoint))
       nil 'time))

(pdefine pl (StartTime start-time) (nodeid) 1 nil ; string -> xml
  "Time when given node started executing."
  (pl-timepoint-value nodeid "EXECUTING" "START"))

(pdefine pl (NodeState node-state) (nodeid) 1 nil
  "Specifies the state of the node with the given ID."
  (xml "NodeStateVariable" (xml "NodeId" nodeid)))

(pdefine pl (NodeOutcome node-outcome) (nodeid) 1 nil
  "Specifies the outcome of the node with the given ID."
  (xml "NodeOutcomeVariable" (xml "NodeId" nodeid)))

(pdefine pl (isKnown IsKnown is-known) (v) 1 nil ; xml -> xml
  ("Returns true or false depending on whether the value of the given declared "
   "variable, node state variable, node outcome variable, or node timepoint "
   "value is known.")
  (xml "IsKnown" v nil 'boolean))


;;; -------------------- Simulation Script -------------------------------------

(insert-plexil-heading
 "== PLEXIL Simulation Script =="
 "This section describes the forms (constructs) that comprise simulation ."
 "scripts, which are used to test plans with the Universal Executive.")

(pdefine ps (PlexilScript plexil-script) (&rest forms) 0 nil     ; list(xml) -> xml
  ("The top level form for a script. "
   "A script must contain exactly one of these, and nothing else.")
  (xml "PLEXILScript" forms))

(pdefine ps (InitialState initial-state) (&rest forms) 0 nil ; list(xml) -> xml
  ("Defines the initial state section of the script, which is optional. "
   "It consists of {{State}} forms.")
  (xml "InitialState" forms))

(pdefine ps (Script script) (&rest forms) 0 nil        ; list(xml) -> xml
  ("Defines the \"script\" section of the script, which is required. "
   "It consists of any of the following forms.")
  (xml "Script" forms))

(insert-plexil-heading
 "NOTE: In the following 5 forms that have a {{value}} or {{result}} argument,  "
 "the argument may be either a single value or a list of values.  The list "
 "must not be quoted, e.g. {{(1 2 3)}}.  A list indicates that that return value "
 "or result is an array.  Its elements must all be of the same type.  For boolean "
 "arrays, you must use 0 and 1 for true and false, respectively.")

(pdefine-syntax ps (State state) (name type value &rest params) 3 nil
  ;; string * string * any * list(xml) -> xml
  "Sets the state of given name and type and parameters to the given value(s)."
  `(let ((actual-value (plexil-handle-boolean ',value)))
     (xml "State" (append
                   (mapcar #'eval ',params)
                   (mapcar (lambda (x) (xml "Value" x))
                           (if (consp actual-value)
                               actual-value
                             (list actual-value))))
          (list (cons "name" ,name) (cons "type" ,type)))))

(defun plexil-handle-boolean (x) ; any -> any
  (if (and (consp x)
           (eq 'quote (car x)))
      (cadr x)
    x))

(pdefine-syntax ps (CommandAck command-ack) (name type result &rest params) 3 nil
  ("Acknowledges the named command with given parameters, "
   "returning the given result(s) of given type.  IMPORTANT NOTE: "
   "An acknowledgment should return only 0 or 1 (false or true). "
   "If your plan is awaiting an actual return value, you must have the script "
   "return this value //before// acknowledgement.  I.e. put one of the "
   "following two forms before this one.")
  `(plexil-command-form "CommandAck" ,name ,type ',result ',params))

(pdefine-syntax ps (Command command) (name type result &rest params) 3 nil
  ("Simulates the completion of the named command with given parameters "
   "returning the given result(s) of given type.")
  `(plexil-command-form "Command" ,name ,type ',result ',params))

(pdefine-syntax ps (CommandAbort command-abort) (name type result &rest params) 3 nil
  ("Simulates the abort of the named command with given parameters "
   "returning the given result(s) of given type.")
  `(plexil-command-form "CommandAbort" ,name ,type ',result ',params))

(pdefine-syntax ps (FunctionCall function-call) (name type result &rest params) 3 nil
  ("Simulates the completion of the named function with given parameters "
   "returning the given result(s) of given type.")
  `(plexil-command-form "FunctionCall" ,name ,type ',result ',params))

(defun plexil-command-form (kind name type result params)
  ;; string * string * string * (any + list(any)) * list(any) -> xml
  (xml kind (append (mapcar #'infer-type (mapcar #'eval params))
                    (mapcar (lambda (x) (xml "Result" x))
                            (if (consp result) result (list result))))
       (list (cons "name" name) (cons "type" type))))

(pdefine ps (Param param) (value &optional type) 1 nil ; any * opt(string) -> xml
  "Defines a parameter //value//, with optional type."
  (xml "Param" value (if type (list (cons "type" type)))))

(pdefine ps (ParamString param-string) (str &rest strs) 1 nil
  ;; string * list(strings) -> xml
  "Like the above, but useful for long strings (they are concatenated)."
  (ps-param (concat str (apply #'concat strs)) "string"))

(pdefine ps (UpdateAck update-ack) (name) 1 nil           ; string -> xml
  "Acknowledges a plan update."
  (xml "UpdateAck" nil (list (cons "name" name))))

(pdefine ps (Simultaneous simultaneous) (&rest forms) 0 nil  ; list(xml) -> xml
  "Wraps script actions that should occur simultaneously."
  (xml "Simultaneous" forms))

(pdefine ps (Comment comment) (&rest sentences) 1 nil   ; list(string) -> xml
  ;; XML comments
  ("This creates a comment in the XML, and may occur in any number within "
   "a script.  It is useful for commenting your script in a way that will "
   "also be reflected in the XML.")
  (xml (apply #'concat sentences) nil nil 'comment))

(insert-plexil-heading
 "=== Simulation Script Shortcuts ==="
 "This section describes various shortcuts for simulation scripts.  "
 "They each expand into some combination of the forms in the previous section.")

(pdefine-syntax ps (CommandSuccess command-success) (name &rest params) 1 nil
  ("Sends a COMMAND_SUCCESS handle for the given command invocation. "
   "NOTE: If your plan is awaiting a return value from the command itself, "
   "you must return this (using the {{Command}} form) //before// "
   "this handle.")
  `(plexil-command-form "CommandAck" ,name "string" "COMMAND_SUCCESS" ',params))

;;; Misc

(defun infer-type (x &optional prefer-float?)                   ; any -> xml
  ;; Type inference for literals
  (cond ((equal 'true x) (pl-boolval 1))
        ((equal 'false x) (pl-boolval 0))
        ((equal 'unknown x) (pl-boolval "UNKNOWN"))
        ((integerp x) (if prefer-float? (pl-realval x) (pl-intval x)))
        ((floatp x) (pl-realval x))
        ((stringp x) (pl-stringval x))
        (t x)))

(defun plexil-unique-node-id (prefix)  ; string -> string
  (setq *node-id-number* (1+ *node-id-number*))
  (format "%s_%s" prefix *node-id-number*))


;;; --------------------------- XML support ------------------------------------

;;; ADT for a simple typed XML.

;;; XML-specific types:
;;;   xml      = <see constructor below>
;;;   attr     = string.string     (cons string string)
;;;   xml-type = symbol (legal values are boolean, string, integer, real,
;;;                      time, number, any, nil, comment
;;;              Semantics: 'any will match any type; nil means untyped
;;;                         if comment, 'name' will be the comment's content

(defun xml (name &optional data attrs type) ; -> xml
  (list
   '*xml*
   name                                 ; string
   data                                 ; xml | list(xml)
   attrs                                ; list(attr)
   type                                 ; xml-type
   ))

(defun xml? (x)                         ; any -> bool
  (and (consp x)
       (eq '*xml* (car x))))

(defun xml-name (x) (second x))
(defun xml-data (x) (third x))
(defun xml-attrs (x) (fourth x))
(defun xml-type (x) (fifth x))

(defun number? (x)                     ; xml -> bool
  (memq (xml-type x) '(number integer real time any)))

(defun real? (x)                        ; xml -> bool
  (memq (xml-type x) '(real any)))

(defun integer? (x)                     ; xml -> bool
  (memq (xml-type x) '(integer any)))

(defun string? (x)                      ; xml -> bool
  (memq (xml-type x) '(string any)))

(defun time? (x)                        ; xml -> bool
  (memq (xml-type x) '(time any)))

(defun boolean? (x)                     ; xml -> bool
  (memq (xml-type x) '(boolean any)))

(defun comment? (x)                     ; xml -> bool
  (eq 'comment (xml-type x)))

(defun xml->string (x)                  ; xml -> string
  (let ((name (xml-name x))
        (data (xml-data x))
        (attrs (xml-attrs x)))
    (if (comment? x)
        (format "<!-- %s -->" name)
      (format "<%s%s>%s%s"
              name
              (if attrs
                  (format " %s" (attributes-string attrs))
                "")
              (cond ((null data) "")
                    ((atom data) (format "%s" data))
                    ((xml? data) (xml->string data))
                    ((list data)
                     (apply #'concat (mapcar (lambda (x) (xml->string x)) data)))
                    (t (error "Unexpected kind of XML data: %s" data)))
              (format "</%s>" name)))))

(defun attributes-string (attrs)
  ;; list(attr) -> string
  (if (null attrs)
      ""
    (format "%s=%S %s"
            (caar attrs)
            (cdar attrs)
            (attributes-string (cdr attrs)))))

(defun xml-comments (sentences)         ; list(string) -> xml
  (if (null sentences) 
      ""
    (concat (xml-comment (car sentences)) (xml-comments (cdr sentences)))))

(defun xml-comment (x)                  ; string -> xml
  (concat "<!-- " x " -->"))

(defun xml-assoc-element (name x &optional deep enough)
  ;; string * (xml + list(xml)) * bool * bool -> any
  ;;
  ;; If x is an XML structure, or list of the same, return the content
  ;; of the first element with the given name.  Search goes down one
  ;; level unless 'deep' is true, in which case the entire tree is
  ;; searched preorder.  'enough' is a bookkeeping variable used to
  ;; control recursion.
  ;;
  (cond ((and (xml? x) (equal name (xml-name x)))
         (xml-data x))
        ((xml? x)
         (unless enough (xml-assoc-element name (xml-data x) deep enough)))
        ((consp x)
         (or (xml-assoc-element name (car x) deep (not deep))
             (xml-assoc-element name (cdr x) deep (not deep))))))

(defun xml-replace-element (name val x &optional deep enough)
  ;; string * any * (xml + list(xml) * bool * bool -> xml
  ;;
  ;; If x is an XML structure, or list of the same, replace the content
  ;; of the first element of the given name with 'val'.  Search goes
  ;; down one level unless 'deep' is true, in which case the entire tree
  ;; is searched preorder.  'enough' is a bookkeeping variable used to
  ;; control recursion.
  ;;
  (cond ((and (xml? x) (equal name (xml-name x)))
         (xml name val (xml-attrs x) (xml-type x)))
        ((xml? x)
         (if enough
             x
           (xml (xml-name x)
                (xml-replace-element name val (xml-data x) deep enough)
                (xml-attrs x) (xml-type x))))
        ((consp x)
         (cons (xml-replace-element name val (car x) deep (not deep))
               (xml-replace-element name val (cdr x) deep (not deep))))
        (t x)))


;;; Misc utilities

(defun indentation (level)              ; int -> string
  ;; two-space tabs  (ascii 32 = space)
  (make-string (* 2 level) 32))

(defun first (x) (car x))
(defun second (x) (cadr x))
(defun third (x) (cadr (cdr x)))
(defun fourth (x) (cadr (cddr x)))
(defun fifth (x) (cadr (cdr (cddr x))))

(defun remove-nil (&rest elts)   ; list -> list
  (remove nil elts))

(defun filter (pred x)
  (if x
      (cond ((funcall pred (car x)) (cons (car x) (filter pred (cdr x))))
            (t (filter pred (cdr x))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; Remainder of this file: Emacs mode for Plexilisp.

;;; Derived from 'scheme.el' in the distribution of GNU Emacs 22.0.92.1.
;;; It is modified under the terms of the GNU General Public License as
;;; published by the Free Software Foundation (version 2 or later).

;;; This needs much refinement!

(require 'lisp-mode)

(defvar plexilisp-mode-abbrev-table nil)

(define-abbrev-table 'plexilisp-mode-abbrev-table ())

(defun plexilisp-mode-variables ()
  (set-syntax-table lisp-mode-syntax-table)
  (setq local-abbrev-table plexilisp-mode-abbrev-table)
  (make-local-variable 'paragraph-start)
  (setq paragraph-start (concat "$\\|" page-delimiter))
  (make-local-variable 'paragraph-separate)
  (setq paragraph-separate paragraph-start)
  (make-local-variable 'paragraph-ignore-fill-prefix)
  (setq paragraph-ignore-fill-prefix t)
  (make-local-variable 'fill-paragraph-function)
  (setq fill-paragraph-function 'lisp-fill-paragraph)
  ;; Adaptive fill mode gets in the way of auto-fill,
  ;; and should make no difference for explicit fill
  ;; because lisp-fill-paragraph should do the job.
  (make-local-variable 'adaptive-fill-mode)
  (setq adaptive-fill-mode nil)
  (make-local-variable 'normal-auto-fill-function)
  (setq normal-auto-fill-function 'lisp-mode-auto-fill)
  (make-local-variable 'indent-line-function)
  (setq indent-line-function 'lisp-indent-line)
  (make-local-variable 'parse-sexp-ignore-comments)
  (setq parse-sexp-ignore-comments t)
  (make-local-variable 'comment-start)
  (setq comment-start ";")
  (set (make-local-variable 'comment-add) 1)
  (make-local-variable 'comment-start-skip)
  ;; Look within the line for a ; following an even number of backslashes
  ;; after either a non-backslash or the line beginning.
  (setq comment-start-skip "\\(\\(^\\|[^\\\\\n]\\)\\(\\\\\\\\\\)*\\);+[ \t]*")
  (make-local-variable 'comment-column)
  (setq comment-column 40)
  (make-local-variable 'parse-sexp-ignore-comments)
  (setq parse-sexp-ignore-comments t)
  (make-local-variable 'lisp-indent-function)
  (setq lisp-indent-function 'plexilisp-indent-function)
  (setq mode-line-process '("" plexilisp-mode-line-process)))


(defvar plexilisp-mode-line-process "")

(defvar plexilisp-mode-map
  (let ((smap (make-sparse-keymap))
	(map (make-sparse-keymap "Plexilisp")))
    (set-keymap-parent smap lisp-mode-shared-map)
    (define-key smap [menu-bar plexilisp] (cons "Plexilisp" map))
    (define-key map [run-plexilisp] '("Run Inferior Plexilisp" . run-plexilisp))
    (define-key map [uncomment-region]
      '("Uncomment Out Region" . (lambda (beg end)
                                   (interactive "r")
                                   (comment-region beg end '(4)))))
    (define-key map [comment-region] '("Comment Out Region" . comment-region))
    (define-key map [indent-region] '("Indent Region" . indent-region))
    (define-key map [indent-line] '("Indent Line" . lisp-indent-line))
    (put 'comment-region 'menu-enable 'mark-active)
    (put 'uncomment-region 'menu-enable 'mark-active)
    (put 'indent-region 'menu-enable 'mark-active)
    smap)
  "Keymap for Plexilisp mode.
All commands in `lisp-mode-shared-map' are inherited by this map.")

;; Used by cmuplexilisp
(defun plexilisp-mode-commands (map)
  ;;(define-key map "\t" 'indent-for-tab-command) ; default
  (define-key map "\177" 'backward-delete-char-untabify)
  (define-key map "\e\C-q" 'indent-sexp))

;;;###autoload
(defun plexilisp-mode ()
  "Major mode for editing Plexilisp code."
  (interactive)
  (kill-all-local-variables)
  (use-local-map plexilisp-mode-map)
  (setq major-mode 'plexilisp-mode)
  (setq mode-name "Plexilisp")
  (plexilisp-mode-variables)
  (run-mode-hooks 'plexilisp-mode-hook))

(defgroup plexilisp nil
  "Editing Plexilisp code."
  :link '(custom-group-link :tag "Font Lock Faces group" font-lock-faces)
  :group 'lisp)

(defcustom plexilisp-mode-hook nil
  "Normal hook run when entering `plexilisp-mode'.
See `run-hooks'."
  :type 'hook
  :group 'plexilisp)

;; Not needed while Plexilisp is implemented in Emacs Lisp
;(defcustom plexilisp-program-name "plexilisp"
;  "*Program invoked by the `run-plexilisp' command."
;  :type 'string
;  :group 'plexilisp)

(put 'lambda 'plexilisp-doc-string-elt 2)
;; Docstring's pos in a `define' depends on whether it's a var or fun def.
(put 'define 'plexilisp-doc-string-elt
     (lambda ()
       ;; The function is called with point right after "define".
       (forward-comment (point-max))
       (if (eq (char-after) ?\() 2 0)))

(defvar calculate-lisp-indent-last-sexp)

;; Copied from lisp-indent-function, but with gets of
;; plexilisp-indent-{function,hook}.
(defun plexilisp-indent-function (indent-point state)
  (let ((normal-indent (current-column)))
    (goto-char (1+ (elt state 1)))
    (parse-partial-sexp (point) calculate-lisp-indent-last-sexp 0 t)
    (if (and (elt state 2)
             (not (looking-at "\\sw\\|\\s_")))
        ;; car of form doesn't seem to be a symbol
        (progn
          (if (not (> (save-excursion (forward-line 1) (point))
                      calculate-lisp-indent-last-sexp))
              (progn (goto-char calculate-lisp-indent-last-sexp)
                     (beginning-of-line)
                     (parse-partial-sexp (point)
					 calculate-lisp-indent-last-sexp 0 t)))
          ;; Indent under the list or under the first sexp on the same
          ;; line as calculate-lisp-indent-last-sexp.  Note that first
          ;; thing on that line has to be complete sexp since we are
          ;; inside the innermost containing sexp.
          (backward-prefix-chars)
          (current-column))
      (let ((function (buffer-substring (point)
					(progn (forward-sexp 1) (point))))
	    method)
	(setq method (or (get (intern-soft function) 'plexilisp-indent-function)
			 (get (intern-soft function) 'plexilisp-indent-hook)))
	(cond ((or (eq method 'defun)
		   (and (null method)
			(> (length function) 3)
			(string-match "\\`def" function)))
	       (lisp-indent-defform state indent-point))
	      ((integerp method)
	       (lisp-indent-specform method state
				     indent-point normal-indent))
	      (method
		(funcall method state indent-point normal-indent)))))))

(defun plexilisp-let-indent (state indent-point normal-indent)
  (skip-chars-forward " \t")
  (if (looking-at "[-a-zA-Z0-9+*/?!@$%^&_:~]")
      (lisp-indent-specform 2 state indent-point normal-indent)
    (lisp-indent-specform 1 state indent-point normal-indent)))

;;; Not sure if this file is the best place for this, but seems okay.
;;;
(setq auto-mode-alist
          (append '(("\\.pli$"  . plexilisp-mode)) auto-mode-alist))

(define-key plexilisp-mode-map "\C-c\C-c" 'plexil)

(provide 'plexilisp)
