// This file is generated automatically. Don't edit it.
#include <string>
#include "v8.h"
#include "extdll.h"
#include "node/utils.hpp"
#include "structures/structures.hpp"

extern enginefuncs_t	 g_engfuncs;

// nodemod.eng.precacheModel();
void sf_eng_pfnPrecacheModel(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnPrecacheModel)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.precacheSound();
void sf_eng_pfnPrecacheSound(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnPrecacheSound)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.setModel();
void sf_eng_pfnSetModel(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnSetModel)(structures::unwrapEntity(isolate, info[0]),
utils::js2string(isolate, info[1]));
}

// nodemod.eng.modelIndex();
void sf_eng_pfnModelIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnModelIndex)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.modelFrames();
void sf_eng_pfnModelFrames(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnModelFrames)(info[0]->Int32Value(context).ToChecked())));
}

// nodemod.eng.setSize();
void sf_eng_pfnSetSize(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnSetSize)(structures::unwrapEntity(isolate, info[0]),
nullptr /* const float * */,
nullptr /* const float * */);
}

// nodemod.eng.changeLevel();
void sf_eng_pfnChangeLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnChangeLevel)(utils::js2string(isolate, info[0]),
utils::js2string(isolate, info[1]));
}

// nodemod.eng.getSpawnParms();
void sf_eng_pfnGetSpawnParms(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnGetSpawnParms)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.eng.saveSpawnParms();
void sf_eng_pfnSaveSpawnParms(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnSaveSpawnParms)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.eng.vecToYaw();
void sf_eng_pfnVecToYaw(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnVecToYaw)(nullptr /* const float * */)));
}

// nodemod.eng.vecToAngles();
void sf_eng_pfnVecToAngles(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnVecToAngles)(nullptr /* const float * */,
nullptr /* float * */);
}

// nodemod.eng.moveToOrigin();
void sf_eng_pfnMoveToOrigin(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnMoveToOrigin)(structures::unwrapEntity(isolate, info[0]),
nullptr /* const float * */,
info[2]->NumberValue(context).ToChecked(),
info[3]->Int32Value(context).ToChecked());
}

// nodemod.eng.changeYaw();
void sf_eng_pfnChangeYaw(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnChangeYaw)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.eng.changePitch();
void sf_eng_pfnChangePitch(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnChangePitch)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.eng.findEntityByString();
void sf_eng_pfnFindEntityByString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnFindEntityByString)(structures::unwrapEntity(isolate, info[0]),
utils::js2string(isolate, info[1]),
utils::js2string(isolate, info[2]))));
}

// nodemod.eng.getEntityIllum();
void sf_eng_pfnGetEntityIllum(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnGetEntityIllum)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.eng.findEntityInSphere();
void sf_eng_pfnFindEntityInSphere(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnFindEntityInSphere)(structures::unwrapEntity(isolate, info[0]),
nullptr /* const float * */,
info[2]->NumberValue(context).ToChecked())));
}

// nodemod.eng.findClientInPVS();
void sf_eng_pfnFindClientInPVS(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnFindClientInPVS)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.eng.entitiesInPVS();
void sf_eng_pfnEntitiesInPVS(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnEntitiesInPVS)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.eng.makeVectors();
void sf_eng_pfnMakeVectors(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnMakeVectors)(nullptr /* const float * */);
}

// nodemod.eng.angleVectors();
void sf_eng_pfnAngleVectors(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnAngleVectors)(nullptr /* const float * */,
nullptr /* float * */,
nullptr /* float * */,
nullptr /* float * */);
}

// nodemod.eng.createEntity();
void sf_eng_pfnCreateEntity(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnCreateEntity)()));
}

// nodemod.eng.removeEntity();
void sf_eng_pfnRemoveEntity(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnRemoveEntity)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.eng.createNamedEntity();
void sf_eng_pfnCreateNamedEntity(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnCreateNamedEntity)(info[0]->Int32Value(context).ToChecked())));
}

// nodemod.eng.makeStatic();
void sf_eng_pfnMakeStatic(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnMakeStatic)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.eng.entIsOnFloor();
void sf_eng_pfnEntIsOnFloor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnEntIsOnFloor)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.eng.dropToFloor();
void sf_eng_pfnDropToFloor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnDropToFloor)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.eng.walkMove();
void sf_eng_pfnWalkMove(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnWalkMove)(structures::unwrapEntity(isolate, info[0]),
info[1]->NumberValue(context).ToChecked(),
info[2]->NumberValue(context).ToChecked(),
info[3]->Int32Value(context).ToChecked())));
}

// nodemod.eng.setOrigin();
void sf_eng_pfnSetOrigin(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnSetOrigin)(structures::unwrapEntity(isolate, info[0]),
nullptr /* const float * */);
}

// nodemod.eng.emitSound();
void sf_eng_pfnEmitSound(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnEmitSound)(structures::unwrapEntity(isolate, info[0]),
info[1]->Int32Value(context).ToChecked(),
utils::js2string(isolate, info[2]),
info[3]->NumberValue(context).ToChecked(),
info[4]->NumberValue(context).ToChecked(),
info[5]->Int32Value(context).ToChecked(),
info[6]->Int32Value(context).ToChecked());
}

// nodemod.eng.emitAmbientSound();
void sf_eng_pfnEmitAmbientSound(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnEmitAmbientSound)(structures::unwrapEntity(isolate, info[0]),
nullptr /* const float * */,
utils::js2string(isolate, info[2]),
info[3]->NumberValue(context).ToChecked(),
info[4]->NumberValue(context).ToChecked(),
info[5]->Int32Value(context).ToChecked(),
info[6]->Int32Value(context).ToChecked());
}

// nodemod.eng.traceLine();
void sf_eng_pfnTraceLine(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnTraceLine)(nullptr /* const float * */,
nullptr /* const float * */,
info[2]->Int32Value(context).ToChecked(),
structures::unwrapEntity(isolate, info[3]),
nullptr /* TraceResult * */);
}

// nodemod.eng.traceToss();
void sf_eng_pfnTraceToss(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnTraceToss)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapEntity(isolate, info[1]),
nullptr /* TraceResult * */);
}

// nodemod.eng.traceMonsterHull();
void sf_eng_pfnTraceMonsterHull(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnTraceMonsterHull)(structures::unwrapEntity(isolate, info[0]),
nullptr /* const float * */,
nullptr /* const float * */,
info[3]->Int32Value(context).ToChecked(),
structures::unwrapEntity(isolate, info[4]),
nullptr /* TraceResult * */)));
}

// nodemod.eng.traceHull();
void sf_eng_pfnTraceHull(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnTraceHull)(nullptr /* const float * */,
nullptr /* const float * */,
info[2]->Int32Value(context).ToChecked(),
info[3]->Int32Value(context).ToChecked(),
structures::unwrapEntity(isolate, info[4]),
nullptr /* TraceResult * */);
}

// nodemod.eng.traceModel();
void sf_eng_pfnTraceModel(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnTraceModel)(nullptr /* const float * */,
nullptr /* const float * */,
info[2]->Int32Value(context).ToChecked(),
structures::unwrapEntity(isolate, info[3]),
nullptr /* TraceResult * */);
}

// nodemod.eng.traceTexture();
void sf_eng_pfnTraceTexture(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnTraceTexture)(structures::unwrapEntity(isolate, info[0]),
nullptr /* const float * */,
nullptr /* const float * */)).ToLocalChecked());
}

// nodemod.eng.traceSphere();
void sf_eng_pfnTraceSphere(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnTraceSphere)(nullptr /* const float * */,
nullptr /* const float * */,
info[2]->Int32Value(context).ToChecked(),
info[3]->NumberValue(context).ToChecked(),
structures::unwrapEntity(isolate, info[4]),
nullptr /* TraceResult * */);
}

// nodemod.eng.getAimVector();
void sf_eng_pfnGetAimVector(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnGetAimVector)(structures::unwrapEntity(isolate, info[0]),
info[1]->NumberValue(context).ToChecked(),
nullptr /* float * */);
}

// nodemod.eng.serverCommand();
void sf_eng_pfnServerCommand(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnServerCommand)(utils::js2string(isolate, info[0]));
}

// nodemod.eng.serverExecute();
void sf_eng_pfnServerExecute(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnServerExecute)();
}

// nodemod.eng.clientCommand();
void sf_eng_pfnClientCommand(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnClientCommand)(structures::unwrapEntity(isolate, info[0]), utils::js2string(isolate, info[1]));;
}

// nodemod.eng.particleEffect();
void sf_eng_pfnParticleEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnParticleEffect)(nullptr /* const float * */,
nullptr /* const float * */,
info[2]->NumberValue(context).ToChecked(),
info[3]->NumberValue(context).ToChecked());
}

// nodemod.eng.lightStyle();
void sf_eng_pfnLightStyle(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnLightStyle)(info[0]->Int32Value(context).ToChecked(),
utils::js2string(isolate, info[1]));
}

// nodemod.eng.decalIndex();
void sf_eng_pfnDecalIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnDecalIndex)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.pointContents();
void sf_eng_pfnPointContents(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnPointContents)(nullptr /* const float * */)));
}

// nodemod.eng.messageBegin();
void sf_eng_pfnMessageBegin(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnMessageBegin)(info[0]->Int32Value(context).ToChecked(),
info[1]->Int32Value(context).ToChecked(),
nullptr /* const float * */,
structures::unwrapEntity(isolate, info[3]));
}

// nodemod.eng.messageEnd();
void sf_eng_pfnMessageEnd(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnMessageEnd)();
}

// nodemod.eng.writeByte();
void sf_eng_pfnWriteByte(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnWriteByte)(info[0]->Int32Value(context).ToChecked());
}

// nodemod.eng.writeChar();
void sf_eng_pfnWriteChar(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnWriteChar)(info[0]->Int32Value(context).ToChecked());
}

// nodemod.eng.writeShort();
void sf_eng_pfnWriteShort(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnWriteShort)(info[0]->Int32Value(context).ToChecked());
}

// nodemod.eng.writeLong();
void sf_eng_pfnWriteLong(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnWriteLong)(info[0]->Int32Value(context).ToChecked());
}

// nodemod.eng.writeAngle();
void sf_eng_pfnWriteAngle(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnWriteAngle)(info[0]->NumberValue(context).ToChecked());
}

// nodemod.eng.writeCoord();
void sf_eng_pfnWriteCoord(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnWriteCoord)(info[0]->NumberValue(context).ToChecked());
}

// nodemod.eng.writeString();
void sf_eng_pfnWriteString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnWriteString)(utils::js2string(isolate, info[0]));
}

// nodemod.eng.writeEntity();
void sf_eng_pfnWriteEntity(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnWriteEntity)(info[0]->Int32Value(context).ToChecked());
}

// nodemod.eng.cVarRegister();
void sf_eng_pfnCVarRegister(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnCVarRegister)(nullptr /* cvar_t * */);
}

// nodemod.eng.cVarGetFloat();
void sf_eng_pfnCVarGetFloat(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnCVarGetFloat)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.cVarGetString();
void sf_eng_pfnCVarGetString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnCVarGetString)(utils::js2string(isolate, info[0]))).ToLocalChecked());
}

// nodemod.eng.cVarSetFloat();
void sf_eng_pfnCVarSetFloat(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnCVarSetFloat)(utils::js2string(isolate, info[0]),
info[1]->NumberValue(context).ToChecked());
}

// nodemod.eng.cVarSetString();
void sf_eng_pfnCVarSetString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnCVarSetString)(utils::js2string(isolate, info[0]),
utils::js2string(isolate, info[1]));
}

// nodemod.eng.pvAllocEntPrivateData();
void sf_eng_pfnPvAllocEntPrivateData(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnPvAllocEntPrivateData)(structures::unwrapEntity(isolate, info[0]),
info[1]->Int32Value(context).ToChecked()) /* void */));
}

// nodemod.eng.pvEntPrivateData();
void sf_eng_pfnPvEntPrivateData(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnPvEntPrivateData)(structures::unwrapEntity(isolate, info[0])) /* void */));
}

// nodemod.eng.freeEntPrivateData();
void sf_eng_pfnFreeEntPrivateData(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnFreeEntPrivateData)(structures::unwrapEntity(isolate, info[0]));
}

// nodemod.eng.szFromIndex();
void sf_eng_pfnSzFromIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnSzFromIndex)(info[0]->Int32Value(context).ToChecked())).ToLocalChecked());
}

// nodemod.eng.allocString();
void sf_eng_pfnAllocString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnAllocString)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.getVarsOfEnt();
void sf_eng_pfnGetVarsOfEnt(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnGetVarsOfEnt)(structures::unwrapEntity(isolate, info[0])) /* entvars_s  */));
}

// nodemod.eng.pEntityOfEntOffset();
void sf_eng_pfnPEntityOfEntOffset(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnPEntityOfEntOffset)(info[0]->Int32Value(context).ToChecked())));
}

// nodemod.eng.entOffsetOfPEntity();
void sf_eng_pfnEntOffsetOfPEntity(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnEntOffsetOfPEntity)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.eng.indexOfEdict();
void sf_eng_pfnIndexOfEdict(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnIndexOfEdict)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.eng.pEntityOfEntIndex();
void sf_eng_pfnPEntityOfEntIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnPEntityOfEntIndex)(info[0]->Int32Value(context).ToChecked())));
}

// nodemod.eng.findEntityByVars();
void sf_eng_pfnFindEntityByVars(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnFindEntityByVars)(nullptr /* struct entvars_s* */)));
}

// nodemod.eng.getModelPtr();
void sf_eng_pfnGetModelPtr(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnGetModelPtr)(structures::unwrapEntity(isolate, info[0])) /* void */));
}

// nodemod.eng.regUserMsg();
void sf_eng_pfnRegUserMsg(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnRegUserMsg)(utils::js2string(isolate, info[0]),
info[1]->Int32Value(context).ToChecked())));
}

// nodemod.eng.animationAutomove();
void sf_eng_pfnAnimationAutomove(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnAnimationAutomove)(structures::unwrapEntity(isolate, info[0]),
info[1]->NumberValue(context).ToChecked());
}

// nodemod.eng.getBonePosition();
void sf_eng_pfnGetBonePosition(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnGetBonePosition)(structures::unwrapEntity(isolate, info[0]),
info[1]->Int32Value(context).ToChecked(),
nullptr /* float * */,
nullptr /* float * */);
}

// nodemod.eng.functionFromName();
void sf_eng_pfnFunctionFromName(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnFunctionFromName)(utils::js2string(isolate, info[0])) /* void */));
}

// nodemod.eng.nameForFunction();
void sf_eng_pfnNameForFunction(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnNameForFunction)(nullptr /* void * */)).ToLocalChecked());
}

// nodemod.eng.clientPrintf();
void sf_eng_pfnClientPrintf(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnClientPrintf)(structures::unwrapEntity(isolate, info[0]),
*(PRINT_TYPE*)utils::jsToBytes(isolate, info[1]),
utils::js2string(isolate, info[2]));
}

// nodemod.eng.serverPrint();
void sf_eng_pfnServerPrint(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnServerPrint)(utils::js2string(isolate, info[0]));
}

// nodemod.eng.cmdArgs();
void sf_eng_pfnCmd_Args(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnCmd_Args)()).ToLocalChecked());
}

// nodemod.eng.cmdArgv();
void sf_eng_pfnCmd_Argv(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnCmd_Argv)(info[0]->Int32Value(context).ToChecked())).ToLocalChecked());
}

// nodemod.eng.cmdArgc();
void sf_eng_pfnCmd_Argc(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnCmd_Argc)()));
}

// nodemod.eng.getAttachment();
void sf_eng_pfnGetAttachment(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnGetAttachment)(structures::unwrapEntity(isolate, info[0]),
info[1]->Int32Value(context).ToChecked(),
nullptr /* float * */,
nullptr /* float * */);
}

// nodemod.eng.randomLong();
void sf_eng_pfnRandomLong(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnRandomLong)(info[0]->Int32Value(context).ToChecked(),
info[1]->Int32Value(context).ToChecked())));
}

// nodemod.eng.randomFloat();
void sf_eng_pfnRandomFloat(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnRandomFloat)(info[0]->NumberValue(context).ToChecked(),
info[1]->NumberValue(context).ToChecked())));
}

// nodemod.eng.setView();
void sf_eng_pfnSetView(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnSetView)(structures::unwrapEntity(isolate, info[0]),
structures::unwrapEntity(isolate, info[1]));
}

// nodemod.eng.time();
void sf_eng_pfnTime(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnTime)()));
}

// nodemod.eng.crosshairAngle();
void sf_eng_pfnCrosshairAngle(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnCrosshairAngle)(structures::unwrapEntity(isolate, info[0]),
info[1]->NumberValue(context).ToChecked(),
info[2]->NumberValue(context).ToChecked());
}

// nodemod.eng.loadFileForMe();
void sf_eng_pfnLoadFileForMe(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnLoadFileForMe)(utils::js2string(isolate, info[0]),
nullptr /* int * */) /* byte */));
}

// nodemod.eng.freeFile();
void sf_eng_pfnFreeFile(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnFreeFile)(nullptr /* void * */);
}

// nodemod.eng.endSection();
void sf_eng_pfnEndSection(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnEndSection)(utils::js2string(isolate, info[0]));
}

// nodemod.eng.compareFileTime();
void sf_eng_pfnCompareFileTime(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnCompareFileTime)(utils::js2string(isolate, info[0]),
utils::js2string(isolate, info[1]),
nullptr /* int * */)));
}

// nodemod.eng.getGameDir();
void sf_eng_pfnGetGameDir(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnGetGameDir)(utils::js2string(isolate, info[0]));
}

// nodemod.eng.cvarRegisterVariable();
void sf_eng_pfnCvar_RegisterVariable(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnCvar_RegisterVariable)(nullptr /* cvar_t * */);
}

// nodemod.eng.fadeClientVolume();
void sf_eng_pfnFadeClientVolume(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnFadeClientVolume)(structures::unwrapEntity(isolate, info[0]),
info[1]->Int32Value(context).ToChecked(),
info[2]->Int32Value(context).ToChecked(),
info[3]->Int32Value(context).ToChecked(),
info[4]->Int32Value(context).ToChecked());
}

// nodemod.eng.setClientMaxspeed();
void sf_eng_pfnSetClientMaxspeed(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnSetClientMaxspeed)(structures::unwrapEntity(isolate, info[0]),
info[1]->NumberValue(context).ToChecked());
}

// nodemod.eng.createFakeClient();
void sf_eng_pfnCreateFakeClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnCreateFakeClient)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.runPlayerMove();
void sf_eng_pfnRunPlayerMove(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnRunPlayerMove)(structures::unwrapEntity(isolate, info[0]),
nullptr /* const float * */,
info[2]->NumberValue(context).ToChecked(),
info[3]->NumberValue(context).ToChecked(),
info[4]->NumberValue(context).ToChecked(),
info[5]->Int32Value(context).ToChecked(),
info[6]->Int32Value(context).ToChecked(),
info[7]->Int32Value(context).ToChecked());
}

// nodemod.eng.numberOfEntities();
void sf_eng_pfnNumberOfEntities(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnNumberOfEntities)()));
}

// nodemod.eng.getInfoKeyBuffer();
void sf_eng_pfnGetInfoKeyBuffer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnGetInfoKeyBuffer)(structures::unwrapEntity(isolate, info[0]))).ToLocalChecked());
}

// nodemod.eng.infoKeyValue();
void sf_eng_pfnInfoKeyValue(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnInfoKeyValue)(utils::js2string(isolate, info[0]),
utils::js2string(isolate, info[1]))).ToLocalChecked());
}

// nodemod.eng.setKeyValue();
void sf_eng_pfnSetKeyValue(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnSetKeyValue)(utils::js2string(isolate, info[0]),
utils::js2string(isolate, info[1]),
utils::js2string(isolate, info[2]));
}

// nodemod.eng.setClientKeyValue();
void sf_eng_pfnSetClientKeyValue(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnSetClientKeyValue)(info[0]->Int32Value(context).ToChecked(),
utils::js2string(isolate, info[1]),
utils::js2string(isolate, info[2]),
utils::js2string(isolate, info[3]));
}

// nodemod.eng.isMapValid();
void sf_eng_pfnIsMapValid(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnIsMapValid)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.staticDecal();
void sf_eng_pfnStaticDecal(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnStaticDecal)(nullptr /* const float * */,
info[1]->Int32Value(context).ToChecked(),
info[2]->Int32Value(context).ToChecked(),
info[3]->Int32Value(context).ToChecked());
}

// nodemod.eng.precacheGeneric();
void sf_eng_pfnPrecacheGeneric(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnPrecacheGeneric)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.getPlayerUserId();
void sf_eng_pfnGetPlayerUserId(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnGetPlayerUserId)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.eng.buildSoundMsg();
void sf_eng_pfnBuildSoundMsg(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnBuildSoundMsg)(structures::unwrapEntity(isolate, info[0]),
info[1]->Int32Value(context).ToChecked(),
utils::js2string(isolate, info[2]),
info[3]->NumberValue(context).ToChecked(),
info[4]->NumberValue(context).ToChecked(),
info[5]->Int32Value(context).ToChecked(),
info[6]->Int32Value(context).ToChecked(),
info[7]->Int32Value(context).ToChecked(),
info[8]->Int32Value(context).ToChecked(),
nullptr /* const float * */,
structures::unwrapEntity(isolate, info[10]));
}

// nodemod.eng.isDedicatedServer();
void sf_eng_pfnIsDedicatedServer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnIsDedicatedServer)()));
}

// nodemod.eng.cVarGetPointer();
void sf_eng_pfnCVarGetPointer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnCVarGetPointer)(utils::js2string(isolate, info[0])) /* cvar_t  */));
}

// nodemod.eng.getPlayerWONId();
void sf_eng_pfnGetPlayerWONId(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnGetPlayerWONId)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.eng.infoRemoveKey();
void sf_eng_pfnInfo_RemoveKey(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnInfo_RemoveKey)(utils::js2string(isolate, info[0]),
utils::js2string(isolate, info[1]));
}

// nodemod.eng.getPhysicsKeyValue();
void sf_eng_pfnGetPhysicsKeyValue(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnGetPhysicsKeyValue)(structures::unwrapEntity(isolate, info[0]),
utils::js2string(isolate, info[1]))).ToLocalChecked());
}

// nodemod.eng.setPhysicsKeyValue();
void sf_eng_pfnSetPhysicsKeyValue(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnSetPhysicsKeyValue)(structures::unwrapEntity(isolate, info[0]),
utils::js2string(isolate, info[1]),
utils::js2string(isolate, info[2]));
}

// nodemod.eng.getPhysicsInfoString();
void sf_eng_pfnGetPhysicsInfoString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnGetPhysicsInfoString)(structures::unwrapEntity(isolate, info[0]))).ToLocalChecked());
}

// nodemod.eng.precacheEvent();
void sf_eng_pfnPrecacheEvent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnPrecacheEvent)(info[0]->Int32Value(context).ToChecked(),
nullptr /* const char* */) /* TODO: type unsigned short */;
}

// nodemod.eng.playbackEvent();
void sf_eng_pfnPlaybackEvent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnPlaybackEvent)(info[0]->Int32Value(context).ToChecked(),
structures::unwrapEntity(isolate, info[1]),
info[2]->Int32Value(context).ToChecked(),
info[3]->NumberValue(context).ToChecked(),
nullptr /* const float * */,
nullptr /* const float * */,
info[6]->NumberValue(context).ToChecked(),
info[7]->NumberValue(context).ToChecked(),
info[8]->Int32Value(context).ToChecked(),
info[9]->Int32Value(context).ToChecked(),
info[10]->Int32Value(context).ToChecked(),
info[11]->Int32Value(context).ToChecked());
}

// nodemod.eng.setFatPVS();
void sf_eng_pfnSetFatPVS(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnSetFatPVS)(nullptr /* const float * */) /* unsigned char  */));
}

// nodemod.eng.setFatPAS();
void sf_eng_pfnSetFatPAS(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnSetFatPAS)(nullptr /* const float * */) /* unsigned char  */));
}

// nodemod.eng.checkVisibility();
void sf_eng_pfnCheckVisibility(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnCheckVisibility)(structures::unwrapEntity(isolate, info[0]),
nullptr /* unsigned char * */)));
}

// nodemod.eng.deltaSetField();
void sf_eng_pfnDeltaSetField(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnDeltaSetField)(nullptr /* struct delta_s * */,
utils::js2string(isolate, info[1]));
}

// nodemod.eng.deltaUnsetField();
void sf_eng_pfnDeltaUnsetField(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnDeltaUnsetField)(nullptr /* struct delta_s * */,
utils::js2string(isolate, info[1]));
}

// nodemod.eng.getCurrentPlayer();
void sf_eng_pfnGetCurrentPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnGetCurrentPlayer)()));
}

// nodemod.eng.canSkipPlayer();
void sf_eng_pfnCanSkipPlayer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnCanSkipPlayer)(structures::unwrapEntity(isolate, info[0]))));
}

// nodemod.eng.deltaFindField();
void sf_eng_pfnDeltaFindField(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnDeltaFindField)(nullptr /* struct delta_s * */,
utils::js2string(isolate, info[1]))));
}

// nodemod.eng.deltaSetFieldByIndex();
void sf_eng_pfnDeltaSetFieldByIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnDeltaSetFieldByIndex)(nullptr /* struct delta_s * */,
info[1]->Int32Value(context).ToChecked());
}

// nodemod.eng.deltaUnsetFieldByIndex();
void sf_eng_pfnDeltaUnsetFieldByIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnDeltaUnsetFieldByIndex)(nullptr /* struct delta_s * */,
info[1]->Int32Value(context).ToChecked());
}

// nodemod.eng.setGroupMask();
void sf_eng_pfnSetGroupMask(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnSetGroupMask)(info[0]->Int32Value(context).ToChecked(),
info[1]->Int32Value(context).ToChecked());
}

// nodemod.eng.createInstancedBaseline();
void sf_eng_pfnCreateInstancedBaseline(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnCreateInstancedBaseline)(info[0]->Int32Value(context).ToChecked(),
nullptr /* struct entity_state_s * */)));
}

// nodemod.eng.cvarDirectSet();
void sf_eng_pfnCvar_DirectSet(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnCvar_DirectSet)(nullptr /* struct cvar_s * */,
utils::js2string(isolate, info[1]));
}

// nodemod.eng.forceUnmodified();
void sf_eng_pfnForceUnmodified(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnForceUnmodified)(*(FORCE_TYPE*)utils::jsToBytes(isolate, info[0]),
nullptr /* const float * */,
nullptr /* const float * */,
utils::js2string(isolate, info[3]));
}

// nodemod.eng.getPlayerStats();
void sf_eng_pfnGetPlayerStats(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnGetPlayerStats)(structures::unwrapEntity(isolate, info[0]),
nullptr /* int * */,
nullptr /* int * */);
}

// nodemod.eng.voiceGetClientListening();
void sf_eng_pfnVoice_GetClientListening(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Boolean::New(isolate, (*g_engfuncs.pfnVoice_GetClientListening)(info[0]->Int32Value(context).ToChecked(),
info[1]->Int32Value(context).ToChecked())));
}

// nodemod.eng.voiceSetClientListening();
void sf_eng_pfnVoice_SetClientListening(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Boolean::New(isolate, (*g_engfuncs.pfnVoice_SetClientListening)(info[0]->Int32Value(context).ToChecked(),
info[1]->Int32Value(context).ToChecked(),
info[2]->BooleanValue(isolate))));
}

// nodemod.eng.getPlayerAuthId();
void sf_eng_pfnGetPlayerAuthId(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, (*g_engfuncs.pfnGetPlayerAuthId)(structures::unwrapEntity(isolate, info[0]))).ToLocalChecked());
}

// nodemod.eng.sequenceGet();
void sf_eng_pfnSequenceGet(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnSequenceGet)(utils::js2string(isolate, info[0]),
utils::js2string(isolate, info[1])) /* void  */));
}

// nodemod.eng.sequencePickSentence();
void sf_eng_pfnSequencePickSentence(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::External::New(isolate, (*g_engfuncs.pfnSequencePickSentence)(utils::js2string(isolate, info[0]),
info[1]->Int32Value(context).ToChecked(),
nullptr /* int * */) /* void  */));
}

// nodemod.eng.getFileSize();
void sf_eng_pfnGetFileSize(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnGetFileSize)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.getApproxWavePlayLen();
void sf_eng_pfnGetApproxWavePlayLen(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnGetApproxWavePlayLen)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.isCareerMatch();
void sf_eng_pfnIsCareerMatch(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnIsCareerMatch)()));
}

// nodemod.eng.getLocalizedStringLength();
void sf_eng_pfnGetLocalizedStringLength(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnGetLocalizedStringLength)(utils::js2string(isolate, info[0]))));
}

// nodemod.eng.registerTutorMessageShown();
void sf_eng_pfnRegisterTutorMessageShown(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnRegisterTutorMessageShown)(info[0]->Int32Value(context).ToChecked());
}

// nodemod.eng.getTimesTutorMessageShown();
void sf_eng_pfnGetTimesTutorMessageShown(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.pfnGetTimesTutorMessageShown)(info[0]->Int32Value(context).ToChecked())));
}

// nodemod.eng.processTutorMessageDecayBuffer();
void sf_eng_pfnProcessTutorMessageDecayBuffer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnProcessTutorMessageDecayBuffer)(nullptr /* int * */,
info[1]->Int32Value(context).ToChecked());
}

// nodemod.eng.constructTutorMessageDecayBuffer();
void sf_eng_pfnConstructTutorMessageDecayBuffer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnConstructTutorMessageDecayBuffer)(nullptr /* int * */,
info[1]->Int32Value(context).ToChecked());
}

// nodemod.eng.resetTutorMessageDecayData();
void sf_eng_pfnResetTutorMessageDecayData(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnResetTutorMessageDecayData)();
}

// nodemod.eng.queryClientCvarValue();
void sf_eng_pfnQueryClientCvarValue(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnQueryClientCvarValue)(structures::unwrapEntity(isolate, info[0]),
utils::js2string(isolate, info[1]));
}

// nodemod.eng.queryClientCvarValue2();
void sf_eng_pfnQueryClientCvarValue2(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  (*g_engfuncs.pfnQueryClientCvarValue2)(structures::unwrapEntity(isolate, info[0]),
utils::js2string(isolate, info[1]),
info[2]->Int32Value(context).ToChecked());
}

// nodemod.eng.checkParm();
void sf_eng_CheckParm(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(v8::Number::New(isolate, (*g_engfuncs.CheckParm)(utils::js2string(isolate, info[0]),
nullptr /* char ** */)));
}

// nodemod.eng.pEntityOfEntIndexAllEntities();
void sf_eng_pfnPEntityOfEntIndexAllEntities(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	auto isolate = info.GetIsolate();
  v8::Locker locker(isolate);
	v8::HandleScope scope(isolate);
	auto context = isolate->GetCurrentContext();

  info.GetReturnValue().Set(structures::wrapEntity(isolate, (*g_engfuncs.pfnPEntityOfEntIndexAllEntities)(info[0]->Int32Value(context).ToChecked())));
}

static std::pair<std::string, v8::FunctionCallback> engineSpecificFunctions[] = {
  { "precacheModel", sf_eng_pfnPrecacheModel },
  { "precacheSound", sf_eng_pfnPrecacheSound },
  { "setModel", sf_eng_pfnSetModel },
  { "modelIndex", sf_eng_pfnModelIndex },
  { "modelFrames", sf_eng_pfnModelFrames },
  { "setSize", sf_eng_pfnSetSize },
  { "changeLevel", sf_eng_pfnChangeLevel },
  { "getSpawnParms", sf_eng_pfnGetSpawnParms },
  { "saveSpawnParms", sf_eng_pfnSaveSpawnParms },
  { "vecToYaw", sf_eng_pfnVecToYaw },
  { "vecToAngles", sf_eng_pfnVecToAngles },
  { "moveToOrigin", sf_eng_pfnMoveToOrigin },
  { "changeYaw", sf_eng_pfnChangeYaw },
  { "changePitch", sf_eng_pfnChangePitch },
  { "findEntityByString", sf_eng_pfnFindEntityByString },
  { "getEntityIllum", sf_eng_pfnGetEntityIllum },
  { "findEntityInSphere", sf_eng_pfnFindEntityInSphere },
  { "findClientInPVS", sf_eng_pfnFindClientInPVS },
  { "entitiesInPVS", sf_eng_pfnEntitiesInPVS },
  { "makeVectors", sf_eng_pfnMakeVectors },
  { "angleVectors", sf_eng_pfnAngleVectors },
  { "createEntity", sf_eng_pfnCreateEntity },
  { "removeEntity", sf_eng_pfnRemoveEntity },
  { "createNamedEntity", sf_eng_pfnCreateNamedEntity },
  { "makeStatic", sf_eng_pfnMakeStatic },
  { "entIsOnFloor", sf_eng_pfnEntIsOnFloor },
  { "dropToFloor", sf_eng_pfnDropToFloor },
  { "walkMove", sf_eng_pfnWalkMove },
  { "setOrigin", sf_eng_pfnSetOrigin },
  { "emitSound", sf_eng_pfnEmitSound },
  { "emitAmbientSound", sf_eng_pfnEmitAmbientSound },
  { "traceLine", sf_eng_pfnTraceLine },
  { "traceToss", sf_eng_pfnTraceToss },
  { "traceMonsterHull", sf_eng_pfnTraceMonsterHull },
  { "traceHull", sf_eng_pfnTraceHull },
  { "traceModel", sf_eng_pfnTraceModel },
  { "traceTexture", sf_eng_pfnTraceTexture },
  { "traceSphere", sf_eng_pfnTraceSphere },
  { "getAimVector", sf_eng_pfnGetAimVector },
  { "serverCommand", sf_eng_pfnServerCommand },
  { "serverExecute", sf_eng_pfnServerExecute },
  { "clientCommand", sf_eng_pfnClientCommand },
  { "particleEffect", sf_eng_pfnParticleEffect },
  { "lightStyle", sf_eng_pfnLightStyle },
  { "decalIndex", sf_eng_pfnDecalIndex },
  { "pointContents", sf_eng_pfnPointContents },
  { "messageBegin", sf_eng_pfnMessageBegin },
  { "messageEnd", sf_eng_pfnMessageEnd },
  { "writeByte", sf_eng_pfnWriteByte },
  { "writeChar", sf_eng_pfnWriteChar },
  { "writeShort", sf_eng_pfnWriteShort },
  { "writeLong", sf_eng_pfnWriteLong },
  { "writeAngle", sf_eng_pfnWriteAngle },
  { "writeCoord", sf_eng_pfnWriteCoord },
  { "writeString", sf_eng_pfnWriteString },
  { "writeEntity", sf_eng_pfnWriteEntity },
  { "cVarRegister", sf_eng_pfnCVarRegister },
  { "cVarGetFloat", sf_eng_pfnCVarGetFloat },
  { "cVarGetString", sf_eng_pfnCVarGetString },
  { "cVarSetFloat", sf_eng_pfnCVarSetFloat },
  { "cVarSetString", sf_eng_pfnCVarSetString },
  { "pvAllocEntPrivateData", sf_eng_pfnPvAllocEntPrivateData },
  { "pvEntPrivateData", sf_eng_pfnPvEntPrivateData },
  { "freeEntPrivateData", sf_eng_pfnFreeEntPrivateData },
  { "szFromIndex", sf_eng_pfnSzFromIndex },
  { "allocString", sf_eng_pfnAllocString },
  { "getVarsOfEnt", sf_eng_pfnGetVarsOfEnt },
  { "pEntityOfEntOffset", sf_eng_pfnPEntityOfEntOffset },
  { "entOffsetOfPEntity", sf_eng_pfnEntOffsetOfPEntity },
  { "indexOfEdict", sf_eng_pfnIndexOfEdict },
  { "pEntityOfEntIndex", sf_eng_pfnPEntityOfEntIndex },
  { "findEntityByVars", sf_eng_pfnFindEntityByVars },
  { "getModelPtr", sf_eng_pfnGetModelPtr },
  { "regUserMsg", sf_eng_pfnRegUserMsg },
  { "animationAutomove", sf_eng_pfnAnimationAutomove },
  { "getBonePosition", sf_eng_pfnGetBonePosition },
  { "functionFromName", sf_eng_pfnFunctionFromName },
  { "nameForFunction", sf_eng_pfnNameForFunction },
  { "clientPrintf", sf_eng_pfnClientPrintf },
  { "serverPrint", sf_eng_pfnServerPrint },
  { "cmdArgs", sf_eng_pfnCmd_Args },
  { "cmdArgv", sf_eng_pfnCmd_Argv },
  { "cmdArgc", sf_eng_pfnCmd_Argc },
  { "getAttachment", sf_eng_pfnGetAttachment },
  { "randomLong", sf_eng_pfnRandomLong },
  { "randomFloat", sf_eng_pfnRandomFloat },
  { "setView", sf_eng_pfnSetView },
  { "time", sf_eng_pfnTime },
  { "crosshairAngle", sf_eng_pfnCrosshairAngle },
  { "loadFileForMe", sf_eng_pfnLoadFileForMe },
  { "freeFile", sf_eng_pfnFreeFile },
  { "endSection", sf_eng_pfnEndSection },
  { "compareFileTime", sf_eng_pfnCompareFileTime },
  { "getGameDir", sf_eng_pfnGetGameDir },
  { "cvarRegisterVariable", sf_eng_pfnCvar_RegisterVariable },
  { "fadeClientVolume", sf_eng_pfnFadeClientVolume },
  { "setClientMaxspeed", sf_eng_pfnSetClientMaxspeed },
  { "createFakeClient", sf_eng_pfnCreateFakeClient },
  { "runPlayerMove", sf_eng_pfnRunPlayerMove },
  { "numberOfEntities", sf_eng_pfnNumberOfEntities },
  { "getInfoKeyBuffer", sf_eng_pfnGetInfoKeyBuffer },
  { "infoKeyValue", sf_eng_pfnInfoKeyValue },
  { "setKeyValue", sf_eng_pfnSetKeyValue },
  { "setClientKeyValue", sf_eng_pfnSetClientKeyValue },
  { "isMapValid", sf_eng_pfnIsMapValid },
  { "staticDecal", sf_eng_pfnStaticDecal },
  { "precacheGeneric", sf_eng_pfnPrecacheGeneric },
  { "getPlayerUserId", sf_eng_pfnGetPlayerUserId },
  { "buildSoundMsg", sf_eng_pfnBuildSoundMsg },
  { "isDedicatedServer", sf_eng_pfnIsDedicatedServer },
  { "cVarGetPointer", sf_eng_pfnCVarGetPointer },
  { "getPlayerWONId", sf_eng_pfnGetPlayerWONId },
  { "infoRemoveKey", sf_eng_pfnInfo_RemoveKey },
  { "getPhysicsKeyValue", sf_eng_pfnGetPhysicsKeyValue },
  { "setPhysicsKeyValue", sf_eng_pfnSetPhysicsKeyValue },
  { "getPhysicsInfoString", sf_eng_pfnGetPhysicsInfoString },
  { "precacheEvent", sf_eng_pfnPrecacheEvent },
  { "playbackEvent", sf_eng_pfnPlaybackEvent },
  { "setFatPVS", sf_eng_pfnSetFatPVS },
  { "setFatPAS", sf_eng_pfnSetFatPAS },
  { "checkVisibility", sf_eng_pfnCheckVisibility },
  { "deltaSetField", sf_eng_pfnDeltaSetField },
  { "deltaUnsetField", sf_eng_pfnDeltaUnsetField },
  { "getCurrentPlayer", sf_eng_pfnGetCurrentPlayer },
  { "canSkipPlayer", sf_eng_pfnCanSkipPlayer },
  { "deltaFindField", sf_eng_pfnDeltaFindField },
  { "deltaSetFieldByIndex", sf_eng_pfnDeltaSetFieldByIndex },
  { "deltaUnsetFieldByIndex", sf_eng_pfnDeltaUnsetFieldByIndex },
  { "setGroupMask", sf_eng_pfnSetGroupMask },
  { "createInstancedBaseline", sf_eng_pfnCreateInstancedBaseline },
  { "cvarDirectSet", sf_eng_pfnCvar_DirectSet },
  { "forceUnmodified", sf_eng_pfnForceUnmodified },
  { "getPlayerStats", sf_eng_pfnGetPlayerStats },
  { "voiceGetClientListening", sf_eng_pfnVoice_GetClientListening },
  { "voiceSetClientListening", sf_eng_pfnVoice_SetClientListening },
  { "getPlayerAuthId", sf_eng_pfnGetPlayerAuthId },
  { "sequenceGet", sf_eng_pfnSequenceGet },
  { "sequencePickSentence", sf_eng_pfnSequencePickSentence },
  { "getFileSize", sf_eng_pfnGetFileSize },
  { "getApproxWavePlayLen", sf_eng_pfnGetApproxWavePlayLen },
  { "isCareerMatch", sf_eng_pfnIsCareerMatch },
  { "getLocalizedStringLength", sf_eng_pfnGetLocalizedStringLength },
  { "registerTutorMessageShown", sf_eng_pfnRegisterTutorMessageShown },
  { "getTimesTutorMessageShown", sf_eng_pfnGetTimesTutorMessageShown },
  { "processTutorMessageDecayBuffer", sf_eng_pfnProcessTutorMessageDecayBuffer },
  { "constructTutorMessageDecayBuffer", sf_eng_pfnConstructTutorMessageDecayBuffer },
  { "resetTutorMessageDecayData", sf_eng_pfnResetTutorMessageDecayData },
  { "queryClientCvarValue", sf_eng_pfnQueryClientCvarValue },
  { "queryClientCvarValue2", sf_eng_pfnQueryClientCvarValue2 },
  { "checkParm", sf_eng_CheckParm },
  { "pEntityOfEntIndexAllEntities", sf_eng_pfnPEntityOfEntIndexAllEntities }
};
v8::Local<v8::ObjectTemplate> registerEngineFunctions(v8::Isolate* isolate) {
  v8::Local <v8::ObjectTemplate> object = v8::ObjectTemplate::New(isolate);
  for (auto &routine : engineSpecificFunctions) {
    object-> Set(v8::String::NewFromUtf8(isolate, routine.first.c_str(), v8::NewStringType::kNormal).ToLocalChecked(), v8::FunctionTemplate::New(isolate, routine.second));
  };
  
  return object;
};

// FAILED (Cannot read properties of undefined (reading 'map')): void	(*pfnAlertMessage)( ALERT_TYPE atype, const char *szFmt, ... );
// FAILED (Cannot read properties of undefined (reading 'map')): void	(*pfnEngineFprintf)( FILE *pfile, const char *szFmt, ... );
// FAILED (Cannot read properties of undefined (reading 'map')): void	(*pfnDeltaAddEncoder)( const char *name, void (*conditionalencode)( struct delta_s *pFields, const unsigned char *from, const unsigned char *to ) );
// FAILED (Cannot read properties of undefined (reading 'map')): void	(*pfnAddServerCommand)( const char *cmd_name, void (*function) (void) );