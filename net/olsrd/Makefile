# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=olsrd
PKG_VERSION:=0.5.2
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://www.olsr.org/releases/0.5
PKG_MD5SUM:=a61029e11c001be2997c1e088576d0f5
PKG_CAT:=bzcat

PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

define Package/olsrd
  SECTION:=net
  CATEGORY:=Network
  TITLE:=OLSR (Optimized Link State Routing) daemon
  URL:=http://www.olsr.org/
  MENU:=1
endef

define Package/olsrd/conffiles
/etc/olsrd.conf
endef

define Package/olsrd-mod-dot-draw
  $(call Package/olsrd)
  DEPENDS:=olsrd
  TITLE:=Dot topology information plugin
  MENU:=0
endef

define Package/olsrd-mod-bmf
  $(call Package/olsrd)
  DEPENDS:=olsrd +libpthread +kmod-tun
  TITLE:=Basic multicast forwarding plugin
  MENU:=0
endef

define Package/olsrd-mod-dyn-gw
  $(call Package/olsrd)
  DEPENDS:=olsrd +libpthread
  TITLE:=Dynamic internet gateway plugin
  MENU:=0
endef

define Package/olsrd-mod-httpinfo
  $(call Package/olsrd)
  DEPENDS:=olsrd
  TITLE:=Small informative web server plugin
  MENU:=0
endef

define Package/olsrd-mod-nameservice
  $(call Package/olsrd)
  DEPENDS:=olsrd
  TITLE:=Lightweight hostname resolver plugin
  MENU:=0
endef

define Package/olsrd-mod-secure
  $(call Package/olsrd)
  DEPENDS:=olsrd
  TITLE:=Message signing plugin to secure routing domain
  MENU:=0
endef

define Package/olsrd-mod-secure/conffiles
/etc/olsrd.d/olsrd_secure_key
endef

define Package/olsrd-mod-tas
  $(call Package/olsrd)
  DEPENDS:=olsrd
  TITLE:=Tiny Application Server (TAS) plugin
  MENU:=0
endef

define Build/Configure
endef

define Build/Compile
	rm -rf $(PKG_INSTALL_DIR)
	mkdir -p $(PKG_INSTALL_DIR)
	$(MAKE) -C "$(PKG_BUILD_DIR)" \
		$(TARGET_CONFIGURE_OPTS) \
		NODEBUG=1 \
		OFLAGS="$(TARGET_CFLAGS)" \
		OS="linux" \
		INSTALL_PREFIX="$(PKG_INSTALL_DIR)" \
		LIBDIR="$(PKG_INSTALL_DIR)/usr/lib" \
		SBINDIR="$(PKG_INSTALL_DIR)/usr/sbin/" \
		ETCDIR="$(PKG_INSTALL_DIR)/etc" \
		MANDIR="$(PKG_INSTALL_DIR)/usr/share/man" \
		STRIP="/bin/true" \
		all libs install install_libs
endef

define Package/olsrd/install
	$(INSTALL_DIR) $(1)/etc
	$(CP) $(PKG_INSTALL_DIR)/etc/olsrd.conf $(1)/etc/
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_INSTALL_DIR)/usr/sbin/olsrd $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/olsrd.init $(1)/etc/init.d/olsrd
endef

define Package/olsrd-mod-dot-draw/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/olsrd_dot_draw.so.* $(1)/usr/lib/
endef

define Package/olsrd-mod-bmf/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/olsrd_bmf.so.* $(1)/usr/lib/
endef

define Package/olsrd-mod-dyn-gw/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/olsrd_dyn_gw.so.* $(1)/usr/lib/
endef

define Package/olsrd-mod-httpinfo/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/olsrd_httpinfo.so.* $(1)/usr/lib/
endef

define Package/olsrd-mod-nameservice/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/olsrd_nameservice.so.* $(1)/usr/lib/
endef

define Package/olsrd-mod-secure/install
	$(INSTALL_DIR) $(1)/etc/olsrd.d
	$(CP) ./files/olsrd_secure_key $(1)/etc/olsrd.d/
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/olsrd_secure.so.* $(1)/usr/lib/
endef

define Package/olsrd-mod-tas/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/olsrd_tas.so.* $(1)/usr/lib/
endef


$(eval $(call BuildPackage,olsrd))
$(eval $(call BuildPackage,olsrd-mod-dot-draw))
$(eval $(call BuildPackage,olsrd-mod-bmf))
$(eval $(call BuildPackage,olsrd-mod-dyn-gw))
$(eval $(call BuildPackage,olsrd-mod-httpinfo))
$(eval $(call BuildPackage,olsrd-mod-nameservice))
$(eval $(call BuildPackage,olsrd-mod-secure))
$(eval $(call BuildPackage,olsrd-mod-tas))
