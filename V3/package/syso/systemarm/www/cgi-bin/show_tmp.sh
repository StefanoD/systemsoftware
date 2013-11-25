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
echo '<div>'
echo "<h1>Files and Folders in $tmp_path</h1>"
echo '<p>'
ls -ag $tmp_path | sed ':a;N;$!ba;s/\n/\<br\>/g'
echo '</p>'
echo '</div>'
echo '</body>'
echo '</html>'
exit 0
