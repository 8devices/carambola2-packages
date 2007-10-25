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
PKG_SOURCE_URL:=http://xorg.freedesktop.org/releases/X11R7.2/src/lib/
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_BUILD_DIR=$(BUILD_DIR)/Xorg/$(_CATEGORY)/${PKG_NAME}-$(PKG_VERSION)/

include $(INCLUDE_DIR)/package.mk

define Package/@NAME@
  SECTION:=xorg-libraries
  CATEGORY:=Xorg
  SUBMENU:=libraries
  DEPENDS:=+xorg-headers-native +util-macros @DEP@ @DISPLAY_SUPPORT
  TITLE:=@NAME@
  URL:=http://xorg.freedesktop.org/
endef

define Build/InstallDev
	DESTDIR=$(STAGING_DIR) $(MAKE) -C $(PKG_BUILD_DIR)/ $(MAKE_FLAGS) install
endef

ifeq (@NAME@,libX11)
 CONFIGURE_ARGS_XTRA=--without-xcb
endif

define libX11/Compile
	$(MAKE_VARS) \
		$(MAKE) -C $(PKG_BUILD_DIR)/src/util CFLAGS="" LDFLAGS="" CC="cc" makekeys 
endef

define libXt/Compile
	$(MAKE_VARS) \
		$(MAKE) -C $(PKG_BUILD_DIR)/util CFLAGS="" LDFLAGS="" CC="cc" 
endef

define Build/Compile
	$(call $(PKG_NAME)/Compile)
	make -C $(PKG_BUILD_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	DESTDIR=$(PKG_INSTALL_DIR) $(MAKE) -C $(PKG_BUILD_DIR) $(MAKE_FLAGS) install
	find $(PKG_INSTALL_DIR) -name *.la | xargs rm -rf
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR)/$(CONFIGURE_PATH); \
	if [ -x $(CONFIGURE_CMD) ]; then \
		$(CP) $(SCRIPT_DIR)/config.{guess,sub} $(PKG_BUILD_DIR)/ && \
		$(CONFIGURE_VARS) \
		$(CONFIGURE_CMD) \
		$(CONFIGURE_ARGS_XTRA) \
		$(CONFIGURE_ARGS) \
		--enable-malloc0returnsnull; \
	fi \
	)
endef

define Package/@NAME@/install/Default
	$(INSTALL_DIR) $(1)/usr/lib
	find $(PKG_INSTALL_DIR)/usr/lib/ -name lib*so* | $(XARGS) -i -t cp -P {} $(1)/usr/lib 
endef

define Package/@NAME@/install
	$(call Package/@NAME@/install/Default,$(1))
endef

define Package/libXaw/install
	$(call Package/@NAME@/install/Default,$(1))
	cd $(1)/usr/lib; rm -f libXaw.so.7 libXaw.so.6; ln -s libXaw7.so.7.0.0 libXaw.so.7; ln -s libXaw6.so.6.0.1 libXaw.so.6
endef

define Build/InstallDev
	$(CP) $(PKG_INSTALL_DIR)/* $(STAGING_DIR)
endef

$(eval $(call BuildPackage,@NAME@))
