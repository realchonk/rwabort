PREFIX = /usr/local
CFLAGS = -O2 -Wall -Wextra

all: rwabort librwabort.so

clean:
	rm -f rwabort librwabort.so

install: rwabort librwabort.so
	mkdir -p ${DESTDIR}${PREFIX}/bin ${DESTDIR}${PREFIX}/lib
	cp -f rwabort ${DESTDIR}${PREFIX}/bin/
	cp -f librwabort.so ${DESTDIR}${PREFIX}/lib/

rwabort: rwabort.sh
	sed 's,@prefix@,${PREFIX},' < rwabort.sh >$@
	chmod +x $@

librwabort.so: rwabort.c
	${CC} -shared -o $@ rwabort.c -fPIC ${CFLAGS} ${LDFLAGS}
