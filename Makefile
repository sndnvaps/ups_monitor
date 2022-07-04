include $(TOPDIR)/rules.mk

PKG_NAME:=ups_monitor
PKG_VERSION:=0.1
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/ups_monitor
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Tools for monitor the ups status
  DEPENDS:=+libwiringPi +libstdcpp
endef

define Package/ups_monitor/description
  Tools for monitor the ups status by i2c interface in Raspberry Pi
endef

define Build/Compile 
	$(MAKE) -C $(PKG_BUILD_DIR) \
	CXX=$(TARGET_CXX) \
	DESTDIR="$(STAGING_DIR)/usr" PREFIX="/"
endef

define Package/ups_monitor/install
	$(INSTALL_DIR) $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ups_monitor	$(1)/usr/bin/
endef

$(eval $(call BuildPackage,ups_monitor))
