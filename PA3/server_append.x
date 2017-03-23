program APPENDPROG
{
	version APPENDVERS
	{
		void RPC_INITAPPENDSERVER(string<64>) = 1;
		int RPC_APPEND(char) = 2;
	} = 1;
} = 0x20000001;
