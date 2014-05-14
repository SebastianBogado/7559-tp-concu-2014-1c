#include "sighandlers/sigint.h"

SIGINT_Handler::SIGINT_Handler() {
	_quit = 0;
}

int SIGINT_Handler::handleSignal(int signum) {
	assert(signum == SIGINT);
	_quit = 1;
	return 0;
}

sig_atomic_t SIGINT_Handler::gate() const {
	return _quit;
}

void SIGINT_Handler::block () {
	sigset_t sa;
	sigemptyset(&sa);
	sigaddset(&sa, SIGINT);
	sigprocmask(SIG_BLOCK, &sa , NULL);
}

void SIGINT_Handler::unblock () {
	sigset_t sa;
	sigemptyset(&sa);
	sigaddset(&sa, SIGINT);
	sigprocmask(SIG_UNBLOCK, &sa , NULL);
}