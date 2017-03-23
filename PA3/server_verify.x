program VERIFYPROG
{
	version VERIFYVERS
	{
		void RPC_INITVERIFYSERVER(string<3>) = 1;
		string RPC_GETSEG(int) = 2;
		string RPC_GETSTRING(void) = 3;
	} = 1;
} = 0x20000002;
