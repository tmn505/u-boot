#!/bin/bash

target=$(pwd)
repo=$(basename ${target})

index=$(ls --color=never --group-directories-first -1 -p --hide=index.html ${target} | awk '{print "<a href=\"./"$1"\">"$1"</a>"}')

echo "<!DOCTYPE html>
<html>
<head><title>Index of ${repo}/</title></head>
<body>
<h1>Index of ${repo}/</h1><hr><pre>
<a href="../">../</a>
${index}
</pre><hr></body>
</html>" > index.html
