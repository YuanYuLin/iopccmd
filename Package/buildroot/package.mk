################################################################################
#
# iopccmd
#
################################################################################

IOPCCMD_VERSION       = <BUILDROOT_VERSION>
IOPCCMD_SITE          = $(call github,YuanYuLin,iopccmd,$(IOPCCMD_VERSION))
IOPCCMD_LICENSE       = GPLv2+
IOPCCMD_LICENSE_FILES = COPYING

IOPCCMD_DEPENDENCIES  = host-pkgconf libiopccmd_client
IOPCCMD_PACKAGE_DIR  = $(BASE_DIR)/packages/iopccmd

IOPCCMD_EXTRA_CFLAGS =                                        \
	-DTARGET_LINUX -DTARGET_POSIX                           \


IOPCCMD_MAKE_ENV =                          \
	CROSS_COMPILE=$(TARGET_CROSS)       \
	BUILDROOT=$(TOP_DIR)                \
	SDKSTAGE=$(STAGING_DIR)             \
	TARGETFS=$(IOPCCMD_PACKAGE_DIR)     \
	TOOLCHAIN=$(HOST_DIR)/usr           \
	HOST=$(GNU_TARGET_NAME)             \
	SYSROOT=$(STAGING_DIR)              \
	JOBS=$(PARALLEL_JOBS)               \
	$(TARGET_CONFIGURE_OPTS)            \
	CFLAGS="$(TARGET_CFLAGS) $(IOPCCMD_EXTRA_CFLAGS)"

define IOPCCMD_BUILD_CMDS
	$(IOPCCMD_MAKE_ENV) $(MAKE) -C $(@D)
endef

define IOPCCMD_INSTALL_TARGET_DIR
	mkdir -p $(TARGET_DIR)/usr/local/bin/
	cp -avr $(IOPCCMD_PACKAGE_DIR)/usr/local/bin/iopccmd $(TARGET_DIR)/usr/local/bin/iopccmd
endef

define IOPCCMD_INSTALL_TARGET_CMDS
	mkdir -p $(IOPCCMD_PACKAGE_DIR)/usr/local/bin/
	$(INSTALL) -m 0755 -D $(@D)/iopccmd.elf	$(IOPCCMD_PACKAGE_DIR)/usr/local/bin/iopccmd
	$(IOPCCMD_INSTALL_TARGET_DIR)
endef

$(eval $(generic-package))
