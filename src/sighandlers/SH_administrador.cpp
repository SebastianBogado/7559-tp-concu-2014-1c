#include "sighandlers/SH_administrador.h"

Administrador_SIGINT_Handler::Administrador_SIGINT_Handler() {
	_quit = 0;
}

int Administrador_SIGINT_Handler::handleSignal(int signum) {
	assert(signum == SIGINT);
	_quit = 1;
	return 0;
}

sig_atomic_t Administrador_SIGINT_Handler::gate() const {
	return _quit;
}

void Administrador_SIGINT_Handler::block () {
	sigset_t sa;
	sigemptyset(&sa);
	sigaddset(&sa, SIGINT);
	sigprocmask(SIG_BLOCK, &sa , NULL);
}

void Administrador_SIGINT_Handler::unblock () {
	sigset_t sa;
	sigemptyset(&sa);
	sigaddset(&sa, SIGINT);
	sigprocmask(SIG_UNBLOCK, &sa , NULL);
}