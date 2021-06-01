#include "D2Handlers.h"
#include "D2Ptrs.h"

void __declspec(naked) GameDraw_Interception()
{
	__asm
	{
		call GameDraw;
		pop ebx;
		mov esp, ebp;
		pop ebp;
		ret;
	}
}

void __declspec(naked) GameAutomapDraw_Interception()
{
   __asm
   {
		 pushad;
		 call GameAutomapDraw;
		 popad;
		 pop edi;
		 pop esi;
		 ret;
   }
} 

//VOID __declspec(naked) OOGDraw_Interception()
//{
//	__asm
//	{
//		PUSHAD
//		CALL OOGDraw
//		POPAD
//		MOV ESI,DWORD PTR DS:[ESI+0x3C]
//		TEST ESI, ESI
//		RETN
//	}
//}

void __declspec(naked) GameLoop_Interception()
{
	__asm
	{
		pushad;
		call GameLoop;
		popad;
		push D2CLIENT_GameLoop_I;
		ret;
	}
}

void __declspec(naked) GamePacketRecv_Interception() {
	__asm
	{
		pushad;

		call GamePacketRecv;
		test eax, eax;

		popad;
		jnz OldCode;

		mov edx, 0;

	OldCode:
		mov eax, [esp + 4];
		push eax;
		call D2NET_ReceivePacket_I;

		ret 4;
	}
}

//VOID __declspec(naked) ChatPacketRecv_Interception()
//{
//   __asm
//   {
//      pushad;
//      call ChatPacketRecv;
//      TEST EAX,EAX;
//
//      popad;
//      jnz oldCall;
//
//        MOV EAX,0;
//      MOV DWORD PTR DS:[EBX+0x6FF3F100],1
//      ret;
//oldCall:
//        CALL eax;
//        MOV DWORD PTR DS:[EBX+0x6FF3F100],1
//        ret;
//
//   }
//}

//VOID __declspec(naked) RealmPacketRecv_Interception()
//{
//	__asm
//	{
//		LEA ECX,DWORD PTR SS:[ESP+4]
//		PUSHAD
//		CALL RealmPacketRecv
//		CMP EAX, 0
//		POPAD
//		JE Block
//		CALL EAX
//Block:
//		RETN
//	}
//}

void __declspec(naked) GameInput_Interception(void)
{
	__asm {
		pushad;
		mov ecx, ebx;
		call GameInput;
		cmp eax, 0;
		popad;
		je Block;
		call D2CLIENT_InputCall_I;
		ret;
	Block:
		xor eax, eax;
		ret;
	}
}

//void __declspec(naked) ChannelInput_Interception(void)
//{
//	__asm
//	{
//		push ecx
//		mov ecx, esi
//
//		call ChannelInput
//
//		test eax, eax
//		pop ecx
//
//		jz SkipInput
//		call D2MULTI_ChannelInput_I
//
//SkipInput:
//		ret
//	}
//}

//void __declspec(naked) ChannelWhisper_Interception(void)
//{
//	__asm
//	{
//		push ecx
//		push edx
//		mov ecx, edi
//		mov edx, ebx
//
//		call ChatHandler
//
//		test eax, eax
//		pop edx
//		pop ecx
//
//		jz SkipWhisper
//		jmp D2MULTI_ChannelWhisper_I
//
//SkipWhisper:
//		ret 4
//	}
//}

//void __declspec(naked) ChannelChat_Interception(void)
//{
//	__asm
//	{
//		push ecx
//		push edx
//		mov ecx, dword ptr ss:[esp+0xC]
//		mov edx, dword ptr ss:[esp+0x10]
//
//		call ChatHandler
//
//		test eax, eax
//		pop edx
//		pop ecx
//
//		jz SkipChat
//		sub esp, 0x308
//		jmp D2MULTI_ChannelChat_I
//
//SkipChat:
//		ret 8
//	}
//}

//void __declspec(naked) ChannelEmote_Interception(void)
//{
//	__asm
//	{
//		push ecx
//		push edx
//		mov ecx, dword ptr ss:[esp+0xC]
//		mov edx, dword ptr ss:[esp+0x10]
//
//		call ChatHandler
//
//		test eax, eax
//		pop edx
//		pop ecx
//
//		jz SkipChat
//		sub esp, 0x4F8
//		jmp D2MULTI_ChannelEmote_I
//
//SkipChat:
//		ret 8
//	}
//}