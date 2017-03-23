program VERIFYPROG
{
	version VERIFYVERS
	{
		void RPC_INITVERIFYSERVER(string<3>) = 1;
		string RPC_GETSEG(int) = 2;
	} = 1;
} = 0x20000002;
