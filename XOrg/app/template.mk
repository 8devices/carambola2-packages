# 
# Copyright (C) 2007 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# blogic@openwrt.org 

include $(TOPDIR)/rules.mk

PKG_BASE_NAME:=@BASE_NAME@
PKG_NAME:=@NAME@
PKG_RELEASE:=1
PKG_VERSION:=@VER@
PKG_SOURCE_URL:=http://xorg.freedesktop.org/releases/X11R7.2/src/app
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/${PKG_NAME}-$(PKG_VERSION)/

_DEPEND:=@DEP@
ifneq (@NAME@,xinit)
_DEPEND+=xorg-server-essentials
endif

include $(INCLUDE_DIR)/package.mk

define Package/@NAME@
  SECTION:=xorg-app
  CATEGORY:=Xorg
  SUBMENU:=app
  DEPENDS:=${_DEPEND} @DISPLAY_SUPPORT
  TITLE:=@NAME@
  URL:=http://xorg.freedesktop.org/
endef

define Build/InstallDev
	DESTDIR=$(STAGING_DIR) $(MAKE) -C $(PKG_BUILD_DIR)  $(MAKE_FLAGS) install
endef

ifeq (@NAME@,xdm)
CONFIGURE_ARGS+=--with-random-device=/dev/urandom
endif

CONFIGURE_ARGS+=LIBS="-Wl,-rpath-link=$(STAGING_DIR)/usr/lib" 

define Build/Compile
	make -C $(PKG_BUILD_DIR)
	make -C $(PKG_BUILD_DIR) DESTDIR=$(PKG_INSTALL_DIR) install
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR)/$(CONFIGURE_PATH); \
	if [ -x $(CONFIGURE_CMD) ]; then \
		$(CP) $(SCRIPT_DIR)/config.{guess,sub} $(PKG_BUILD_DIR)/ && \
		$(CONFIGURE_VARS) \
		$(CONFIGURE_CMD) \
		$(CONFIGURE_ARGS_XTRA) \
		$(CONFIGURE_ARGS) ;\
	fi \
	)
endef

define Package/@NAME@/install
	$(INSTALL_DIR) $(1)
	$(CP) $(PKG_INSTALL_DIR)/* $(1)
	rm -rf $(1)/usr/man/
endef

define Package/xinit/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/xinit $(1)/usr/bin/xinit
	cd $(1)/usr/bin/; ln -s xinit startx
endef

$(eval $(call BuildPackage,@NAME@))
