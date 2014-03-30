#
# Copyright (C) 2005-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=gnutls
PKG_VERSION:=3.2.12
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=ftp://ftp.gnutls.org/gcrypt/gnutls/v3.2
PKG_MD5SUM:=f507365940de8f095e1d867c6f0842f6
PKG_MAINTAINER:=Nikos Mavrogiannopoulos <nmav@gnutls.org>

PKG_INSTALL:=1
PKG_LIBTOOL_PATHS:=. lib

include $(INCLUDE_DIR)/package.mk

define Package/gnutls/Default
  SUBMENU:=SSL
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=GNU TLS
  URL:=http://www.gnutls.org/
endef

define Package/gnutls/Default/description
 GnuTLS is a secure communications library implementing the SSL, TLS
 and DTLS protocols and technologies around them. It provides a simple
 C language application programming interface (API) to access the secure
 communications protocols as well as APIs to parse and write X.509, PKCS12,
 OpenPGP and other required structures. It is aimed to be portable and
 efficient with focus on security and interoperability.
endef


define Package/certtool
$(call Package/gnutls/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE+= (certool utility)
  DEPENDS+= +libgnutls
endef

define Package/certtool/description
$(call Package/gnutls/Default/description)
 This package contains the GnuTLS certtool utility.
endef


define Package/gnutls-utils
$(call Package/gnutls/Default)
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE+= (utilities)
  DEPENDS+= +libgnutls
endef

define Package/gnutls-utils/description
$(call Package/gnutls/Default/description)
 This package contains the GnuTLS gnutls-cli, gnutls-serv, psktool,
 and srptool utilities.
endef


define Package/libgnutls
$(call Package/gnutls/Default)
  TITLE+= (library)
  DEPENDS+= +libnettle
endef

define Package/libgnutls/description
$(call Package/gnutls/Default/description)
 This package contains the GnuTLS shared library, needed by other programs.
endef


define Package/libgnutls-openssl
$(call Package/gnutls/Default)
  TITLE+= (OpenSSL compat library)
  DEPENDS+= +libgnutls
endef

define Package/libgnutls-openssl/description
$(call Package/gnutls/Default/description)
 This package contains the GnuTLS OpenSSL compatibility layer shared library.
endef

CONFIGURE_ARGS+= \
	--enable-shared \
	--enable-static \
	--disable-openpgp-authentication \
	--disable-libdane \
	--disable-guile \
	--disable-nls \
	--without-zlib \
	--with-included-libtasn1 \
	--enable-local-libopts \
	--disable-doc \
	--disable-tests \
	--disable-rsa-export \
	--disable-crywrap \
	--without-p11-kit \

# XXX: AM_CFLAGS duplicates with CFLAGS
MAKE_FLAGS:= \
	AM_CFLAGS="" \

TARGET_CFLAGS += $(FPIC)
TARGET_LDFLAGS += -Wl,-rpath-link=$(STAGING_DIR)/usr/lib

define Build/Configure
	$(SED) 's,-I$$$${includedir},,g' $(PKG_BUILD_DIR)/configure
	$(SED) 's,-L$$$${libdir},,g' $(PKG_BUILD_DIR)/configure
	$(call Build/Configure/Default)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include $(1)/usr/lib/pkgconfig
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/include/gnutls \
		$(1)/usr/include/
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/libgnutls{,-openssl}.{a,so*} \
		$(1)/usr/lib/
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/lib/pkgconfig/gnutls.pc \
		$(1)/usr/lib/pkgconfig/
endef

define Package/certtool/conffiles
/etc/gnutls/certtool.cfg
endef

define Package/certtool/install
	$(INSTALL_DIR) $(1)/etc/gnutls
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/doc/certtool.cfg $(1)/etc/gnutls/
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/certtool $(1)/usr/bin/
endef


define Package/gnutls-utils/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(CP) \
		$(PKG_INSTALL_DIR)/usr/bin/gnutls-{cli,serv} \
		$(PKG_INSTALL_DIR)/usr/bin/psktool \
		$(PKG_INSTALL_DIR)/usr/bin/ocsptool \
		$(PKG_INSTALL_DIR)/usr/bin/srptool \
		$(1)/usr/bin/
endef


define Package/libgnutls/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libgnutls.so.* $(1)/usr/lib/
endef


define Package/libgnutls-openssl/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libgnutls-openssl.so.* $(1)/usr/lib/
endef


$(eval $(call BuildPackage,certtool))
$(eval $(call BuildPackage,gnutls-utils))
$(eval $(call BuildPackage,libgnutls))
$(eval $(call BuildPackage,libgnutls-openssl))
