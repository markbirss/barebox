#ifndef __RELOC_H
#define __RELOC_H

#ifdef CONFIG_ARCH_HAS_RELOC

extern unsigned long _u_boot_start, _bss_start, _bss_end;

extern ulong __early_init_data_begin, __early_init_data_end;
extern ulong __early_init_data_size;

/*
 * The difference between our link address and the address we're
 * currently running at.
 */
unsigned long reloc_offset(void);

/*
 * When not running at link address, relocate a pointer by
 * taking reloc_offset() into account
 */
#define RELOC(a) (typeof(*a) *)((unsigned long)a + reloc_offset())

/*
 * dito, used for variables
 */
#define RELOC_VAR(v) *(typeof(v)* )((unsigned long)&v + reloc_offset())

void early_init(void);

/*
 * put a variable into early init RAM. This section will
 * be relocated into SRAM during early init
 */
#define __initdata	__attribute__ ((__section__ (".early_init_data")))

/* Access init data */
#define INITDATA(var) *(typeof(var) *)((ulong)(&var) - \
		(ulong)&__early_init_data_begin + \
		(ulong)RELOC_VAR(init_data_ptr))

extern void *init_data_ptr;

#else

static inline int reloc_offset(void)
{
	return 0;
}

static inline void early_init(void)
{
}

#define RELOC(a) a
#define RELOC_VAR(v) v

#define __initdata

#define INITDATA(var)	var

#endif /* CONFIG_ARCH_HAS_RELOC */

#endif /* __RELOC_H */