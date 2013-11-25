################################################################################
#
# Syso Package für Webserver 
#
################################################################################

SYSO_VERSION = 1.0
# software download
SYSO_SOURCE  = syso-$(SYSO_VERSION).tar.gz
SYSO_SITE    = https://github.com/roroettg/systemsoftware/V3/syso-$(SYSO_VERSION).tar.gz
SYSO_LICENSE = GPL
SYSO_INSTALL_STAGING = YES
#SYSO_CONFIG_SCRIPTS = SYSO-config

# Entpacken wird von Buildroot erledigt

# aktionen die vor dem kompilieren ausgeführt werden
define SYSO_CONFIGURE_CMDS
endef

# aktionen zum kompilieren
define SYSO_BUILD_CMDS
      $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D)
endef

define SYSO_INSTALL_STAGING_CMDS
     $(INSTALL) -D -m 0755 $(@D)/timeinfo $(STAGING_DIR)/usr/lib/timeinfo
     #$(INSTALL) -D -m 0644 $(@D)/foo.h $(STAGING_DIR)/usr/include/foo.h
    #$(INSTALL) -D -m 0755 $(@D)/SYSO.so* $(STAGING_DIR)/usr/lib
 endef
# untar the archive
# apply patches to the sources if necessary

# configure the sources
### schreibe script für zb ip zuweisung oder konfiguration vom server!
# build

# kopiere die fertig kompilierten binaries zum Target RFS

define SYSO_INSTALL_TARGET_CMDS
	if ! [ -d "$(TARGET_DIR)/bin/syso" ]; then \
		    mkdir -p $(TARGET_DIR)/bin/syso/; \
	fi
	$(INSTALL) -D -m 0755 $(@D)/ $(TARGET_DIR)/bin/syso/
endef


define SYSO_CLEAN_CMDS
        $(MAKE) -C $(@D) clean
endef

define SYSO_UNINSTALL_TARGET_CMDS
        rm -rf $(TARGET_DIR)/bin/syso/*
endef

$(eval $(generic-package))
