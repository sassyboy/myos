ENTRY(main)
 
SECTIONS
{
    /* For now let's assume every program starts @ 2MB */
    . = 0x200000;

    _app_start = .;
    _app_text_start = .;
    .text :
    {
        KEEP(*(.text.main))
        *(.text)
    }
    . = ALIGN(4096); /* align to page size */
    _app_text_end = .;
 
    _app_rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(4096); /* align to page size */
    _app_rodata_end = .;
 
    _app_data_start = .;
    .data :
    {
        *(.data)
    }
    . = ALIGN(4096); /* align to page size */
    _app_data_end = .;
 
    _app_bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(4096); /* align to page size */
    _app_bss_end = .;
    _app_bss_size = _app_bss_end - _app_bss_start;
    _app_end = .;
} 
