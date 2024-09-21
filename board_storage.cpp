#include "pch.h"

/* This board stores settings in external plain SPI flash */
#if !defined(EFI_BOOTLOADER) && (EFI_STORAGE_MFS == TRUE)

#include "hal_serial_nor.h"
#include "hal_mfs.h"

/* Some fields in following struct are used for DMA transfers, so do not cache */
NO_CACHE SNORDriver snor1;

static const SPIConfig W25SpiCfg = {
	.end_cb			= NULL,
	.error_cb		= NULL,
	.dcr			= STM32_DCR_FSIZE(23U) |	/* 8MB device.          */
					  STM32_DCR_CSHT(1U)		/* NCS 2 cycles delay.  */
};

static const SNORConfig W25FlashConfig = {
	.busp			= &SPID1,
	.buscfg			= &SPIcfg1
};

const MFSConfig mfsd_nor_config = {
	.flashp			= (BaseFlash *)&snor1,
	.erased			= 0xFFFFFFFFU,
	.bank_size		= 64 * 1024U,
	.bank0_start	= 0U,
	.bank0_sectors	= 128U,	/* 128 * 4 K = 0.5 Mb */
	.bank1_start	= 128U,
	.bank1_sectors	= 128U
};

void boardInitMfs() {
	/* Initializing and starting snor1 driver.*/
	snorObjectInit(&snor1);
	snorStart(&snor1, &snorcfg1);
}

const MFSConfig *boardGetMfsConfig()
{
	return &mfsd_nor_config;
}

#endif /* EFI_STORAGE_MFS == TRUE */
