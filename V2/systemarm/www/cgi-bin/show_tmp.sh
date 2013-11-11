#!/bin/sh


tmp_path="/tmp"


echo "Content-type: text/html"
echo ""
echo '<!DOCTYPE html>'
echo '<head>'
echo '<title>System Informations</title>'
echo '<link rel="stylesheet" href="../style.css"/>'
echo '</head>' 
echo '<body>'
echo "<h1>Files and Folders in $tmp_path</h1>"
ls -ag $tmp_path | sed ':a;N;$!ba;s/\n/\<br\>/g'
echo '</body>'
echo '</html>'
exit 0
