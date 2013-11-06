#!/bin/sh

while [ 1 ]
do    
	echo '<!DOCTYPE html>'
	echo '<head>'
	echo '<title>System Informations</title>'
   echo '<link rel="stylesheet" href="style.css"/>'
	echo '</head>' 
	echo '<body>'
	./sysinfo_html
	echo '</body>'
	echo '</html>'
   sleep 5
done
