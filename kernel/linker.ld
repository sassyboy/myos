ENTRY(_start)

INCLUDE include.ld

SECTIONS
{
    . = LNK_KERNEL_ORG; /* Comes from platform-defs.h of the selected platform */
    _kernel_start = .;

    _kernel_text_start = .;
    .text :
    {
        KEEP(*(.text.boot))
        *(.text)
    }
    . = ALIGN(4096);
    _kernel_text_end = .;
    _kernel_text_size = _kernel_text_end - _kernel_text_start;
 
    _kernel_rodata_start = .;
    .rodata :
    {
        *(.rodata)
    }
    . = ALIGN(4096);
    _kernel_rodata_end = .;
    _kernel_rodata_size = _kernel_rodata_end - _kernel_rodata_start;
 
    _kernel_data_start = .;
    .data :
    {
        *(.data)
    }
    . = ALIGN(4096);
    _kernel_data_end = .;
    _kernel_data_size = _kernel_data_end - _kernel_data_start;
 
    _kernel_bss_start = .;
    .bss :
    {
        bss = .;
        *(.bss)
    }
    . = ALIGN(4096);
    _kernel_bss_end = .;
    _kernel_bss_size = _kernel_bss_end - _kernel_bss_start;
    
    _kernel_end = .;
    _kernel_size = _kernel_end - _kernel_start;
} 
