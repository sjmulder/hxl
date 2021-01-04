#!/bin/bash

make 2> >(awk '
{
	if ($4 ~ /(warning|error)/) {
		gsub(/ /, "", $4)
		print "##vso[task.logissue" \
		    " type="         $4 \
		    ";sourcepath="   $1 \
		    ";linenumber="   $2 \
		    ";columnnumber=" $3 "]" $5
	} else {
		print $0
	}
}
' FS=: OFS=: >&2) $@
