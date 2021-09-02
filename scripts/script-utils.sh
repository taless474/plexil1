# Utilities for shell script command line parsing
# This file is meant to be sourced

# Copyright (c) 2006-2021, Universities Space Research Association (USRA).
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Universities Space Research Association nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
# TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# N.B. "$(basename "$0")" should expand to the script which sourced this one.

plexil_check_prog()
{
    if [ ! -x "$1" ]
    then
        echo "$(basename "$0"): $1 does not exist." >&2
        echo 'You probably need to build it.' >&2
        exit 1
    fi
}

# $1 is option, $2 is value
validate_file()
{
    if [ -z "$2" ]
    then
        echo "$(basename "$0"): Option $1: missing filename" >&2
        usage
        exit 2
    elif [ ! -e "$2" ]
    then
        echo "$(basename "$0"): Option $1: $2 does not exist" >&2
        exit 1
    elif [ -d "$2" ]
    then
        echo "$(basename "$0"): Option $1: $2 is a directory" >&2
        exit 1
    elif [ ! -r "$2" ]
    then
        echo "$(basename "$0"): Option $1: $2 cannot be read" >&2
        exit 1
    fi
}

# $1 is option, $2 is value
validate_host()
{
    if [ -z "$2" ]
    then
        echo "$(basename "$0"): missing host name for $1 option" >&2
        usage
        exit 2
    fi
}

# $1 is option, $2 is value
validate_port()
{
    if [ -z "$1" ]
    then
        echo "$(basename "$0"): Missing parameter for $2 option" >&2
        exit 2
    elif [ "$1" -lt 0 ] || [ "$1" -gt 65535 ]
    then        
        echo "$(basename "$0"): $1 is not a valid port number for $2" >&2
        exit 2
    fi
}

# Confirm that the library file exists and is readable.
# Attempt to emulate exec's behavior.
# $1 is library name, additional args are library paths
# Return status of 0 if found, 1 if not found.
confirm_library_file_exists()
{
    local fname="$1"
    local fname_plx=
    if printf '%s\n' "$1" | grep -qv '\.plx$'
    then
        fname_plx="${1}.plx"
    fi

    if [ -f "$fname" ] && [ -r "$fname" ]
    then
        return 0
    elif [ -n "$fname_plx" ] && [ -f "$fname_plx" ] && [ -r "$fname_plx" ]
    then
        return 0
    fi

    shift
    while [ -n "$1" ]
    do
        if [ -f "$1/$fname" ] && [ -r "$1/$fname" ]
        then
            return 0
        elif [ -n "$fname_plx" ] && [ -f "$fname_plx" ] && [ -r "$fname_plx" ]
        then
            return 0
        fi
    done

    echo "$(basename "$0"): Option -l: Library $fname not found" >&2
    exit 1
}

# Confirm that the file exists and is readable.
# $1 is file, additional args are library paths
# Return status of 0 if found, 1 if not found.
validate_library_file()
{
    if [ -f "$1" ] && [ -r "$1" ]
    then
        return 0
    fi

    local fname="$1"
    shift
    while [ -n "$1" ]
    do
        if [ -f "$1/$fname" ] && [ -r "$1/$fname" ]
        then
            return 0
        fi
    done

}

warn()
{
   if [ -z "$quiet" ]
   then
       echo "$1"
   fi
}

# $1 = the plan file to check
checker()
{
    if [ ! -x "$PLEXIL_HOME/scripts/checkPlexil" ]
    then 
        echo "$(basename "$0"): $PLEXIL_HOME/scripts/checkPlexil not found." >&2
        exit 1
    fi
    if [ ! -r "$1" ]
    then
        echo "$(basename "$0"): File $1 does not exist, or is not readable." >&2
        exit 1
    fi
    echo "Checking plan..."
    if ! "$PLEXIL_HOME/scripts/checkPlexil" "$1"
    then
        echo "$(basename "$0"): $1 contains type errors. See the checker output for details." >&2
        exit 1
    fi
}
