#!/bin/sh
set -e

SRC_OVERLAY="/tmp/trabalho_minix/src_changes"
DEST="/usr/src"

if [ ! -d "$SRC_OVERLAY" ]; then
    echo "ERRO: Pasta $SRC_OVERLAY nao encontrada!"
    exit 1
fi

echo "=== Aplicando alteracoes da syscall set_custom_proc_name em $DEST ==="
cp -v "$SRC_OVERLAY/include/unistd.h" "$DEST/include/unistd.h"
cp -v "$SRC_OVERLAY/minix/include/minix/callnr.h" "$DEST/minix/include/minix/callnr.h"
cp -v "$SRC_OVERLAY/minix/include/minix/com.h" "$DEST/minix/include/minix/com.h"
cp -v "$SRC_OVERLAY/minix/include/minix/ipc.h" "$DEST/minix/include/minix/ipc.h"
cp -v "$SRC_OVERLAY/minix/include/minix/syslib.h" "$DEST/minix/include/minix/syslib.h"
cp -v "$SRC_OVERLAY/minix/lib/libc/sys/Makefile.inc" "$DEST/minix/lib/libc/sys/Makefile.inc"
cp -v "$SRC_OVERLAY/minix/lib/libc/sys/set_custom_proc_name.c" "$DEST/minix/lib/libc/sys/set_custom_proc_name.c"
cp -v "$SRC_OVERLAY/minix/lib/libsys/Makefile" "$DEST/minix/lib/libsys/Makefile"
cp -v "$SRC_OVERLAY/minix/lib/libsys/sys_setname.c" "$DEST/minix/lib/libsys/sys_setname.c"
cp -v "$SRC_OVERLAY/minix/servers/pm/proto.h" "$DEST/minix/servers/pm/proto.h"
cp -v "$SRC_OVERLAY/minix/servers/pm/table.c" "$DEST/minix/servers/pm/table.c"
cp -v "$SRC_OVERLAY/minix/servers/pm/misc.c" "$DEST/minix/servers/pm/misc.c"
cp -v "$SRC_OVERLAY/minix/kernel/system.h" "$DEST/minix/kernel/system.h"
cp -v "$SRC_OVERLAY/minix/kernel/system.c" "$DEST/minix/kernel/system.c"
cp -v "$SRC_OVERLAY/minix/kernel/system/Makefile.inc" "$DEST/minix/kernel/system/Makefile.inc"
cp -v "$SRC_OVERLAY/minix/kernel/system/do_setname.c" "$DEST/minix/kernel/system/do_setname.c"

cp -v /tmp/trabalho_minix/user_test/test_set_proc_name.c /root/test_set_proc_name.c

echo ""
echo "=== Arquivos aplicados com sucesso! ==="
echo "Proximo passo: cd /usr/src && make build MKUPDATE=yes"
