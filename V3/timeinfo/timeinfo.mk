##############################################
#
#	timeinfo
#
##############################################
TIMEINFO_SITE_METHOD=local
TIMEINFO_SITE=~/SystemSoftware/buildroot-2013.08.1/package/timeinfo/src


define TIMEINFO_BUILD_CMDS
		$(MAKE) -C $(@D) all
endef

define TIMEINFO_INSTALL_TARGET_CMDS
		cp $(@D)/* $(TARGET_DIR)/usr/bin/
endef

$(eval $(generic-package))