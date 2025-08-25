// This file is generated automatically. Don't edit it.
/// <reference path="./enums.d.ts" />
/// <reference path="./events.d.ts" />
/// <reference path="./structures.d.ts" />
/// <reference path="./engine.d.ts" />
/// <reference path="./dll.d.ts" />

declare namespace nodemod {
  // Properties
  const cwd: string;
  const players: Entity[];
  const mapname: string;
  const time: number;

  // Event system functions
  function on<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;
  function addEventListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;
  function addListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;
  function removeListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;
  function removeEventListener<T extends keyof EventCallbacks>(eventName: T, callback: EventCallbacks[T]): void;
  function clearListeners(eventName?: keyof EventCallbacks): void;
  function fire<T extends keyof EventCallbacks>(eventName: T, ...args: Parameters<EventCallbacks[T]>): void;

  // Utility functions
  function getUserMsgId(msgName: string): number;
  function getUserMsgName(msgId: number): string;
  function setMetaResult(result: META_RES): void;
  function continueServer(): void;
}