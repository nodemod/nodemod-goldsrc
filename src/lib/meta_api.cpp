#include <extdll.h>
#include <meta_api.h>
#include "node/nodeimpl.hpp"
#include "v8.h"
#include "node/utils.hpp"

#define NODEMOD_VERSION "0.1.0"

meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;
enginefuncs_t *g_pengfuncsTable;

plugin_info_t Plugin_info =
{
	META_INTERFACE_VERSION,			// ifvers
	"NodeMod",				// name
	NODEMOD_VERSION,							// version
	__DATE__,						// date
	"TheEVolk",						// author
	"https://github.com/theevolk/nodemod",						// url
	"NM",						// logtag
	PT_ANYTIME,						// (when) loadable
	PT_ANYTIME,						// (when) unloadable
};

C_DLLEXPORT int Meta_Query(char *interfaceVersion, plugin_info_t **plinfo, mutil_funcs_t *pMetaUtilFuncs)
{
	*plinfo = &Plugin_info;
	gpMetaUtilFuncs = pMetaUtilFuncs;
	return TRUE;
}

META_FUNCTIONS gMetaFunctionTable =
{
	NULL,						// pfnGetEntityAPI		HL SDK; called before game DLL
	NULL,						// pfnGetEntityAPI_Post		META; called after game DLL
	GetEntityAPI2,				// pfnGetEntityAPI2		HL SDK2; called before game DLL
	GetEntityAPI2_Post,			// pfnGetEntityAPI2_Post	META; called after game DLL
	GetNewDLLFunctions,			// pfnGetNewDLLFunctions	HL SDK2; called before game DLL
	GetNewDLLFunctions_Post,	// pfnGetNewDLLFunctions_Post	META; called after game DLL
	GetEngineFunctions,			// pfnGetEngineFunctions	META; called before HL engine
	GetEngineFunctions_Post,	// pfnGetEngineFunctions_Post	META; called after HL engine
};

void nodemod_reload_cmd() {
	g_engfuncs.pfnServerPrint("NodeMod: Manual reload command triggered\n");
	if (nodeImpl.reload()) {
		g_engfuncs.pfnServerPrint("NodeMod: JavaScript environment reloaded successfully!\n");
	} else {
		g_engfuncs.pfnServerPrint("NodeMod: Failed to reload JavaScript environment!\n");
	}
}

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs)
{
	gpMetaGlobals = pMGlobals;
	gpGamedllFuncs = pGamedllFuncs;

	g_engfuncs.pfnServerPrint("\n################\n# Hello World i am NODEMOD! #\n################\n\n");
	nodeImpl.Initialize();
	nodeImpl.loadScript();
	
	// Register the reload command
	REG_SVR_COMMAND("nodemod_reload", nodemod_reload_cmd);
	g_engfuncs.pfnServerPrint("NodeMod: Registered 'nodemod_reload' command\n");

	memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
	return TRUE;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason)
{
	g_engfuncs.pfnServerPrint("Stopping nodemod...\n");
	nodeImpl.Stop();
	return TRUE;
}

void getUserMsgId(const v8::FunctionCallbackInfo<v8::Value>& info) {
	v8::Locker locker(info.GetIsolate());
	v8::HandleScope scope(info.GetIsolate());
	auto context = info.GetIsolate()->GetCurrentContext();

  info.GetReturnValue().Set(
  v8::Number::New(info.GetIsolate(), GET_USER_MSG_ID(&Plugin_info, utils::js2string(info.GetIsolate(), info[0]), NULL))
	);
}

void getUserMsgName(const v8::FunctionCallbackInfo<v8::Value>& info) {
	v8::Locker locker(info.GetIsolate());
	v8::HandleScope scope(info.GetIsolate());
	auto context = info.GetIsolate()->GetCurrentContext();

	const char* msgName = GET_USER_MSG_NAME(&Plugin_info, info[0]->Int32Value(context).ToChecked(), NULL);
	if (msgName != NULL) {
		info.GetReturnValue().Set(
			v8::String::NewFromUtf8(info.GetIsolate(), msgName).ToLocalChecked()
		);
	} else {
		info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), "").ToLocalChecked());
	}
}

void setMetaResult(const v8::FunctionCallbackInfo<v8::Value>& info) {
	v8::Locker locker(info.GetIsolate());
	v8::HandleScope scope(info.GetIsolate());
	auto context = info.GetIsolate()->GetCurrentContext();

	SET_META_RESULT((META_RES)info[0]->Int32Value(context).ToChecked());
}