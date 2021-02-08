#include "CreateAccountState.h"

void Connect::CreateAccountState::CreateAccountButtonFunction()
{
	LOG_TRACE("Create Account Button Pressed");

	// Check user doesn't already exist and then create account
	m_ErrorMessages.SetText("Test Text");
}