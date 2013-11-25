##############################################
#
#	syso
#
##############################################
SYSO_SITE_METHOD=local
SYSO_SITE=~/systemsoftware/V3/package/syso


define SYSO_BUILD_CMDS
		$(MAKE) -C $(@D)/src all
endef

define SYSO_INSTALL_TARGET_CMDS
		cp $(@D)/src/timeinfo $(TARGET_DIR)/usr/bin/
		cp $(@D)/src/timeinfo2 $(TARGET_DIR)/usr/bin/
		mkdir -p -m 755 $(TARGET_DIR)/www/cgi-bin 
		cp -r $(@D)/systemarm/* $(TARGET_DIR)/
endef

$(eval $(generic-package))
