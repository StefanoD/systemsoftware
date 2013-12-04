### Anleitung
1.Copy syso in the buildroot package folder
2. syso1.0 enthält alle benötigten srcfiles scripte und htmlseiten für den linux server.
3. Benötigt anpassung der Pfade im syso/.mk file!
4. Config.in in den package folder von buildroot verschieben!
5. .configs in Buildroot/linux/busybox folder ersten (ConfigToBR)!
6. evtl über make-menuconfig Einstellungen anpassen
7. make clean, make!
