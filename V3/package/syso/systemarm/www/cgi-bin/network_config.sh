#!/bin/sh



echo "Content-type: text/html"
echo ""
echo '<!DOCTYPE html>'
echo '<head>'
echo '<title>Network Configuration</title>'
echo '<link rel="stylesheet" href="../style.css"/>'
echo '</head>' 
echo '<body>'
echo '<div>'
echo "<h1>Network Config: ifconfig -a</h1>"
echo '<p>'
ifconfig -a | sed ':a;N;$!ba;s/\n/\<br\>/g'
echo '</p>'
echo '</div>'
echo '</body>'
echo '</html>'
exit 0
