#pragma once

#include <signal.h>
#include <assert.h>

#include "signal/SignalHandler.h"

class Administrador_SIGINT_Handler : public EventHandler {
	private:
	sig_atomic_t _quit;
	
	public:
	Administrador_SIGINT_Handler();
	virtual int handleSignal(int signum);
	sig_atomic_t gate() const;
	
	void block();
	void unblock();
};
