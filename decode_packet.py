pack=raw_input("Packet:")
#pack="      dY4      FKB       w+C    SRSTUVW XY Z                                                                 a                                                                                                                       b                                                                                                                       c"

def decb64(b64):
    mul=1
    val=0
    for i in b64:
        if i==' ':
            continue
        if i=='+':
            cval=62
        elif i=='/':
            cval=63
        elif i<'A':
            cval=ord(i)-ord('0')+52
        elif i<'a':
            cval=ord(i)-ord('A')+0
        else:
            cval=ord(i)-ord('a')+26
        val+=cval*mul
        mul*=64
    return val
bTime=pack[:9]
bLati=pack[9:18]
bLong=pack[18:28]
bAlti=pack[28:33]
print bTime+':'+bLati+':'+bLong+':'+bAlti+':'+pack[33:]
print 'Time:%d(%s)'%(decb64(bTime),bTime.lstrip())
print 'Lati:%d(%s)'%(decb64(bLati),bLati.lstrip())
print 'Long:%d(%s)'%(decb64(bLong),bLong.lstrip())
print 'Alti:%d(%s)'%(decb64(bAlti),bAlti.lstrip())
