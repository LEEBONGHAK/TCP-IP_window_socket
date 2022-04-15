BOOL IsLittleEndian()
{
    u_short x = 0x1234;
    u_short x2 = htons(x);
    if (x2 == x)
        return FALSE;
    return TRUE;
}