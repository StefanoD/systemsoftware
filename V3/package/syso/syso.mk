##############################################
#
#	syso
#
##############################################
SYSO_SITE_METHOD=local
SYSO_VERSION=1.0
SYSO_SITE=~/systemsoftware/V3/package/syso-$(SYSO_VERSION)

define SYSO_BUILD_CMDS
		mkdir -p $(@D)/src/out
		$(MAKE) -C $(@D)/src all
endef

define SYSO_INSTALL_TARGET_CMDS
		#copy compiled apps
		cp $(@D)/src/out/* $(TARGET_DIR)/usr/bin/
		mkdir -p -m 755 $(TARGET_DIR)/usr/src
		cp $(@D)/src/*.c $(TARGET_DIR)/usr/src
		#copy www files	
		mkdir -p -m 755 $(TARGET_DIR)/www/cgi-bin
		cp -r $(@D)/systemarm/* $(TARGET_DIR)/
		#copy scripts
		cp $(@D)/scripts/interfaces $(TARGET_DIR)/etc/network/interfaces
		cp $(@D)/scripts/init_httpd.sh $(TARGET_DIR)/etc/init.d/S90http_init
		cp $(@D)/scripts/init_tmpfs.sh $(TARGET_DIR)/etc/init.d/S91tmpfs_init
		cp $(@D)/scripts/init_keymap.sh $(TARGET_DIR)/etc/init.d/S92keymap_init
		cp $(@D)/scripts/init_final.sh $(TARGET_DIR)/etc/init.d/S93final	
endef

$(eval $(generic-package))
