macro put_metadata
{

    section metadata, 0x0004
        db @0

    section metadata, 0x0080
        db @1

}

put_metadata("TM Test Program - Register Counter", "Dennis W. Griffin")

section program
    .main:
        ld aw, 0x00
        call n, loop
        ret n

    .loop:
        inc aw
        ret zs
        jmp n, loop
