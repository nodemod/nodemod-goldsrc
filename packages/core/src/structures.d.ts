// This file is generated automatically. Don't edit it.
declare namespace nodemod {
  interface FileHandle {
    // Opaque file handle - use with engine file functions
  }

  /** Entity variables - properties and state of game entities */
  interface Entvars {
    /** ACCESSOR_T(..., "classname", classname, GETSTR, SETSTR) */
    classname: string;

    /** ACCESSOR_T(..., "globalname", globalname, GETSTR, SETSTR) */
    globalname: string;

    /** ACCESSOR_T(..., "target", target, GETSTR, SETSTR) */
    target: string;

    /** ACCESSOR_T(..., "targetname", targetname, GETSTR, SETSTR) */
    targetname: string;

    /** ACCESSOR_T(..., "netname", netname, GETSTR, SETSTR) */
    netname: string;

    /** ACCESSOR_T(..., "message", message, GETSTR, SETSTR) */
    message: string;

    /** ACCESSOR_T(..., "noise", noise, GETSTR, SETSTR) */
    noise: string;

    /** ACCESSOR_T(..., "noise1", noise1, GETSTR, SETSTR) */
    noise1: string;

    /** ACCESSOR_T(..., "noise2", noise2, GETSTR, SETSTR) */
    noise2: string;

    /** ACCESSOR_T(..., "noise3", noise3, GETSTR, SETSTR) */
    noise3: string;

    /** ACCESSOR_T(..., "origin", origin, GETVEC3, SETVEC3) */
    origin: number[];

    /** ACCESSOR_T(..., "oldorigin", oldorigin, GETVEC3, SETVEC3) */
    oldorigin: number[];

    /** ACCESSOR_T(..., "velocity", velocity, GETVEC3, SETVEC3) */
    velocity: number[];

    /** ACCESSOR_T(..., "basevelocity", basevelocity, GETVEC3, SETVEC3) */
    basevelocity: number[];

    /** ACCESSOR_T(..., "clbasevelocity", clbasevelocity, GETVEC3, SETVEC3) */
    clbasevelocity: number[];

    /** ACCESSOR_T(..., "movedir", movedir, GETVEC3, SETVEC3) */
    movedir: number[];

    /** ACCESSOR_T(..., "angles", angles, GETVEC3, SETVEC3) */
    angles: number[];

    /** ACCESSOR_T(..., "avelocity", avelocity, GETVEC3, SETVEC3) */
    avelocity: number[];

    /** ACCESSOR_T(..., "punchangle", punchangle, GETVEC3, SETVEC3) */
    punchangle: number[];

    /** ACCESSOR_T(..., "v_angle", v_angle, GETVEC3, SETVEC3) */
    v_angle: number[];

    /** ACCESSOR_T(..., "endpos", endpos, GETVEC3, SETVEC3) */
    endpos: number[];

    /** ACCESSOR_T(..., "startpos", startpos, GETVEC3, SETVEC3) */
    startpos: number[];

    /** ACCESSOR_T(..., "absmin", absmin, GETVEC3, SETVEC3) */
    absmin: number[];

    /** ACCESSOR_T(..., "absmax", absmax, GETVEC3, SETVEC3) */
    absmax: number[];

    /** ACCESSOR_T(..., "mins", mins, GETVEC3, SETVEC3) */
    mins: number[];

    /** ACCESSOR_T(..., "maxs", maxs, GETVEC3, SETVEC3) */
    maxs: number[];

    /** ACCESSOR_T(..., "size", size, GETVEC3, SETVEC3) */
    size: number[];

    /** ACCESSOR_T(..., "rendercolor", rendercolor, GETVEC3, SETVEC3) */
    rendercolor: number[];

    /** ACCESSOR_T(..., "view_ofs", view_ofs, GETVEC3, SETVEC3) */
    view_ofs: number[];

    /** ACCESSOR_T(..., "vuser1", vuser1, GETVEC3, SETVEC3) */
    vuser1: number[];

    /** ACCESSOR_T(..., "vuser2", vuser2, GETVEC3, SETVEC3) */
    vuser2: number[];

    /** ACCESSOR_T(..., "vuser3", vuser3, GETVEC3, SETVEC3) */
    vuser3: number[];

    /** ACCESSOR_T(..., "vuser4", vuser4, GETVEC3, SETVEC3) */
    vuser4: number[];

    /** ACCESSOR_T(..., "impacttime", impacttime, GETN, SETFLOAT) */
    impacttime: number;

    /** ACCESSOR_T(..., "starttime", starttime, GETN, SETFLOAT) */
    starttime: number;

    /** ACCESSOR_T(..., "idealpitch", idealpitch, GETN, SETFLOAT) */
    idealpitch: number;

    /** ACCESSOR_T(..., "pitch_speed", pitch_speed, GETN, SETFLOAT) */
    pitch_speed: number;

    /** ACCESSOR_T(..., "ideal_yaw", ideal_yaw, GETN, SETFLOAT) */
    ideal_yaw: number;

    /** ACCESSOR_T(..., "yaw_speed", yaw_speed, GETN, SETFLOAT) */
    yaw_speed: number;

    /** ACCESSOR_T(..., "ltime", ltime, GETN, SETFLOAT) */
    ltime: number;

    /** ACCESSOR_T(..., "nextthink", nextthink, GETN, SETFLOAT) */
    nextthink: number;

    /** ACCESSOR_T(..., "gravity", gravity, GETN, SETFLOAT) */
    gravity: number;

    /** ACCESSOR_T(..., "friction", friction, GETN, SETFLOAT) */
    friction: number;

    /** ACCESSOR_T(..., "frame", frame, GETN, SETFLOAT) */
    frame: number;

    /** ACCESSOR_T(..., "animtime", animtime, GETN, SETFLOAT) */
    animtime: number;

    /** ACCESSOR_T(..., "framerate", framerate, GETN, SETFLOAT) */
    framerate: number;

    /** ACCESSOR_T(..., "scale", scale, GETN, SETFLOAT) */
    scale: number;

    /** ACCESSOR_T(..., "renderamt", renderamt, GETN, SETFLOAT) */
    renderamt: number;

    /** ACCESSOR_T(..., "health", health, GETN, SETFLOAT) */
    health: number;

    /** ACCESSOR_T(..., "frags", frags, GETN, SETFLOAT) */
    frags: number;

    /** ACCESSOR_T(..., "takedamage", takedamage, GETN, SETFLOAT) */
    takedamage: number;

    /** ACCESSOR_T(..., "max_health", max_health, GETN, SETFLOAT) */
    max_health: number;

    /** ACCESSOR_T(..., "teleport_time", teleport_time, GETN, SETFLOAT) */
    teleport_time: number;

    /** ACCESSOR_T(..., "armortype", armortype, GETN, SETFLOAT) */
    armortype: number;

    /** ACCESSOR_T(..., "armorvalue", armorvalue, GETN, SETFLOAT) */
    armorvalue: number;

    /** ACCESSOR_T(..., "dmg_take", dmg_take, GETN, SETFLOAT) */
    dmg_take: number;

    /** ACCESSOR_T(..., "dmg_save", dmg_save, GETN, SETFLOAT) */
    dmg_save: number;

    /** ACCESSOR_T(..., "dmg", dmg, GETN, SETFLOAT) */
    dmg: number;

    /** ACCESSOR_T(..., "dmgtime", dmgtime, GETN, SETFLOAT) */
    dmgtime: number;

    /** ACCESSOR_T(..., "speed", speed, GETN, SETFLOAT) */
    speed: number;

    /** ACCESSOR_T(..., "air_finished", air_finished, GETN, SETFLOAT) */
    air_finished: number;

    /** ACCESSOR_T(..., "pain_finished", pain_finished, GETN, SETFLOAT) */
    pain_finished: number;

    /** ACCESSOR_T(..., "radsuit_finished", radsuit_finished, GETN, SETFLOAT) */
    radsuit_finished: number;

    /** ACCESSOR_T(..., "maxspeed", maxspeed, GETN, SETFLOAT) */
    maxspeed: number;

    /** ACCESSOR_T(..., "fov", fov, GETN, SETFLOAT) */
    fov: number;

    /** ACCESSOR_T(..., "flFallVelocity", flFallVelocity, GETN, SETFLOAT) */
    flFallVelocity: number;

    /** ACCESSOR_T(..., "fuser1", fuser1, GETN, SETFLOAT) */
    fuser1: number;

    /** ACCESSOR_T(..., "fuser2", fuser2, GETN, SETFLOAT) */
    fuser2: number;

    /** ACCESSOR_T(..., "fuser3", fuser3, GETN, SETFLOAT) */
    fuser3: number;

    /** ACCESSOR_T(..., "fuser4", fuser4, GETN, SETFLOAT) */
    fuser4: number;

    /** ACCESSOR_T(..., "fixangle", fixangle, GETN, SETINT) */
    fixangle: number;

    /** ACCESSOR_T(..., "modelindex", modelindex, GETN, SETINT) */
    modelindex: number;

    /** ACCESSOR_T(..., "viewmodel", viewmodel, GETN, SETINT) */
    viewmodel: number;

    /** ACCESSOR_T(..., "weaponmodel", weaponmodel, GETN, SETINT) */
    weaponmodel: number;

    /** ACCESSOR_T(..., "movetype", movetype, GETN, SETINT) */
    movetype: number;

    /** ACCESSOR_T(..., "solid", solid, GETN, SETINT) */
    solid: number;

    /** ACCESSOR_T(..., "skin", skin, GETN, SETINT) */
    skin: number;

    /** ACCESSOR_T(..., "body", body, GETN, SETINT) */
    body: number;

    /** ACCESSOR_T(..., "effects", effects, GETN, SETINT) */
    effects: number;

    /** ACCESSOR_T(..., "light_level", light_level, GETN, SETINT) */
    light_level: number;

    /** ACCESSOR_T(..., "sequence", sequence, GETN, SETINT) */
    sequence: number;

    /** ACCESSOR_T(..., "gaitsequence", gaitsequence, GETN, SETINT) */
    gaitsequence: number;

    /** ACCESSOR_T(..., "rendermode", rendermode, GETN, SETINT) */
    rendermode: number;

    /** ACCESSOR_T(..., "renderfx", renderfx, GETN, SETINT) */
    renderfx: number;

    /** ACCESSOR_T(..., "weapons", weapons, GETN, SETINT) */
    weapons: number;

    /** ACCESSOR_T(..., "deadflag", deadflag, GETN, SETINT) */
    deadflag: number;

    /** ACCESSOR_T(..., "button", button, GETN, SETINT) */
    button: number;

    /** ACCESSOR_T(..., "impulse", impulse, GETN, SETINT) */
    impulse: number;

    /** ACCESSOR_T(..., "spawnflags", spawnflags, GETN, SETINT) */
    spawnflags: number;

    /** ACCESSOR_T(..., "flags", flags, GETN, SETINT) */
    flags: number;

    /** ACCESSOR_T(..., "colormap", colormap, GETN, SETINT) */
    colormap: number;

    /** ACCESSOR_T(..., "team", team, GETN, SETINT) */
    team: number;

    /** ACCESSOR_T(..., "waterlevel", waterlevel, GETN, SETINT) */
    waterlevel: number;

    /** ACCESSOR_T(..., "watertype", watertype, GETN, SETINT) */
    watertype: number;

    /** ACCESSOR_T(..., "playerclass", playerclass, GETN, SETINT) */
    playerclass: number;

    /** ACCESSOR_T(..., "weaponanim", weaponanim, GETN, SETINT) */
    weaponanim: number;

    /** ACCESSOR_T(..., "pushmsec", pushmsec, GETN, SETINT) */
    pushmsec: number;

    /** ACCESSOR_T(..., "bInDuck", bInDuck, GETN, SETINT) */
    bInDuck: number;

    /** ACCESSOR_T(..., "flTimeStepSound", flTimeStepSound, GETN, SETINT) */
    flTimeStepSound: number;

    /** ACCESSOR_T(..., "flSwimTime", flSwimTime, GETN, SETINT) */
    flSwimTime: number;

    /** ACCESSOR_T(..., "flDuckTime", flDuckTime, GETN, SETINT) */
    flDuckTime: number;

    /** ACCESSOR_T(..., "iStepLeft", iStepLeft, GETN, SETINT) */
    iStepLeft: number;

    /** ACCESSOR_T(..., "gamestate", gamestate, GETN, SETINT) */
    gamestate: number;

    /** ACCESSOR_T(..., "oldbuttons", oldbuttons, GETN, SETINT) */
    oldbuttons: number;

    /** ACCESSOR_T(..., "groupinfo", groupinfo, GETN, SETINT) */
    groupinfo: number;

    /** ACCESSOR_T(..., "iuser1", iuser1, GETN, SETINT) */
    iuser1: number;

    /** ACCESSOR_T(..., "iuser2", iuser2, GETN, SETINT) */
    iuser2: number;

    /** ACCESSOR_T(..., "iuser3", iuser3, GETN, SETINT) */
    iuser3: number;

    /** ACCESSOR_T(..., "iuser4", iuser4, GETN, SETINT) */
    iuser4: number;

    /** templ->SetNativeDataProperty("model", ...) */
    model: string;

    /** templ->SetNativeDataProperty("chain", ...) */
    chain: Entity | null;

    /** templ->SetNativeDataProperty("dmg_inflictor", ...) */
    dmg_inflictor: Entity | null;

    /** templ->SetNativeDataProperty("enemy", ...) */
    enemy: Entity | null;

    /** templ->SetNativeDataProperty("aiment", ...) */
    aiment: Entity | null;

    /** templ->SetNativeDataProperty("owner", ...) */
    owner: Entity | null;

    /** templ->SetNativeDataProperty("groundentity", ...) */
    groundentity: Entity | null;

    /** templ->SetNativeDataProperty("pContainingEntity", ...) */
    pContainingEntity: Entity | null;

    /** templ->SetNativeDataProperty("euser1", ...) */
    euser1: Entity | null;

    /** templ->SetNativeDataProperty("euser2", ...) */
    euser2: Entity | null;

    /** templ->SetNativeDataProperty("euser3", ...) */
    euser3: Entity | null;

    /** templ->SetNativeDataProperty("euser4", ...) */
    euser4: Entity | null;
  }
  /** Client-specific data sent from server to client each frame */
  interface ClientData {
    /** ACCESSOR_T(..., "origin", origin, GETVEC3, SETVEC3) */
    origin: number[];

    /** ACCESSOR_T(..., "velocity", velocity, GETVEC3, SETVEC3) */
    velocity: number[];

    /** ACCESSOR_T(..., "punchangle", punchangle, GETVEC3, SETVEC3) */
    punchangle: number[];

    /** ACCESSOR_T(..., "view_ofs", view_ofs, GETVEC3, SETVEC3) */
    view_ofs: number[];

    /** ACCESSOR_T(..., "viewmodel", viewmodel, GETN, SETINT) */
    viewmodel: number;

    /** ACCESSOR_T(..., "flags", flags, GETN, SETINT) */
    flags: number;

    /** ACCESSOR_T(..., "waterlevel", waterlevel, GETN, SETINT) */
    waterlevel: number;

    /** ACCESSOR_T(..., "watertype", watertype, GETN, SETINT) */
    watertype: number;

    /** ACCESSOR_T(..., "bInDuck", bInDuck, GETN, SETINT) */
    bInDuck: number;

    /** ACCESSOR_T(..., "weapons", weapons, GETN, SETINT) */
    weapons: number;

    /** ACCESSOR_T(..., "flTimeStepSound", flTimeStepSound, GETN, SETINT) */
    flTimeStepSound: number;

    /** ACCESSOR_T(..., "flDuckTime", flDuckTime, GETN, SETINT) */
    flDuckTime: number;

    /** ACCESSOR_T(..., "flSwimTime", flSwimTime, GETN, SETINT) */
    flSwimTime: number;

    /** ACCESSOR_T(..., "waterjumptime", waterjumptime, GETN, SETINT) */
    waterjumptime: number;

    /** ACCESSOR_T(..., "weaponanim", weaponanim, GETN, SETINT) */
    weaponanim: number;

    /** ACCESSOR_T(..., "m_iId", m_iId, GETN, SETINT) */
    m_iId: number;

    /** ACCESSOR_T(..., "ammo_shells", ammo_shells, GETN, SETINT) */
    ammo_shells: number;

    /** ACCESSOR_T(..., "ammo_nails", ammo_nails, GETN, SETINT) */
    ammo_nails: number;

    /** ACCESSOR_T(..., "ammo_cells", ammo_cells, GETN, SETINT) */
    ammo_cells: number;

    /** ACCESSOR_T(..., "ammo_rockets", ammo_rockets, GETN, SETINT) */
    ammo_rockets: number;

    /** ACCESSOR_T(..., "tfstate", tfstate, GETN, SETINT) */
    tfstate: number;

    /** ACCESSOR_T(..., "pushmsec", pushmsec, GETN, SETINT) */
    pushmsec: number;

    /** ACCESSOR_T(..., "deadflag", deadflag, GETN, SETINT) */
    deadflag: number;

    /** ACCESSOR_T(..., "health", health, GETN, SETFLOAT) */
    health: number;

    /** ACCESSOR_T(..., "maxspeed", maxspeed, GETN, SETFLOAT) */
    maxspeed: number;

    /** ACCESSOR_T(..., "fov", fov, GETN, SETFLOAT) */
    fov: number;

    /** ACCESSOR_T(..., "m_flNextAttack", m_flNextAttack, GETN, SETFLOAT) */
    m_flNextAttack: number;

    /** ACCESSOR_T(..., "iuser1", iuser1, GETN, SETINT) */
    iuser1: number;

    /** ACCESSOR_T(..., "iuser2", iuser2, GETN, SETINT) */
    iuser2: number;

    /** ACCESSOR_T(..., "iuser3", iuser3, GETN, SETINT) */
    iuser3: number;

    /** ACCESSOR_T(..., "iuser4", iuser4, GETN, SETINT) */
    iuser4: number;

    /** ACCESSOR_T(..., "fuser1", fuser1, GETN, SETFLOAT) */
    fuser1: number;

    /** ACCESSOR_T(..., "fuser2", fuser2, GETN, SETFLOAT) */
    fuser2: number;

    /** ACCESSOR_T(..., "fuser3", fuser3, GETN, SETFLOAT) */
    fuser3: number;

    /** ACCESSOR_T(..., "fuser4", fuser4, GETN, SETFLOAT) */
    fuser4: number;

    /** ACCESSOR_T(..., "vuser1", vuser1, GETVEC3, SETVEC3) */
    vuser1: number[];

    /** ACCESSOR_T(..., "vuser2", vuser2, GETVEC3, SETVEC3) */
    vuser2: number[];

    /** ACCESSOR_T(..., "vuser3", vuser3, GETVEC3, SETVEC3) */
    vuser3: number[];

    /** ACCESSOR_T(..., "vuser4", vuser4, GETVEC3, SETVEC3) */
    vuser4: number[];
  }
  /** Entity state for network transmission */
  interface EntityState {
    /** ACCESSOR_T(..., "entityType", entityType, GETN, SETINT) */
    entityType: number;

    /** ACCESSOR_T(..., "number", number, GETN, SETINT) */
    number: number;

    /** ACCESSOR_T(..., "messagenum", messagenum, GETN, SETINT) */
    messagenum: number;

    /** ACCESSOR_T(..., "modelindex", modelindex, GETN, SETINT) */
    modelindex: number;

    /** ACCESSOR_T(..., "sequence", sequence, GETN, SETINT) */
    sequence: number;

    /** ACCESSOR_T(..., "colormap", colormap, GETN, SETINT) */
    colormap: number;

    /** ACCESSOR_T(..., "skin", skin, GETN, SETINT) */
    skin: number;

    /** ACCESSOR_T(..., "solid", solid, GETN, SETINT) */
    solid: number;

    /** ACCESSOR_T(..., "effects", effects, GETN, SETINT) */
    effects: number;

    /** ACCESSOR_T(..., "eflags", eflags, GETN, SETINT) */
    eflags: number;

    /** ACCESSOR_T(..., "rendermode", rendermode, GETN, SETINT) */
    rendermode: number;

    /** ACCESSOR_T(..., "renderamt", renderamt, GETN, SETINT) */
    renderamt: number;

    /** ACCESSOR_T(..., "renderfx", renderfx, GETN, SETINT) */
    renderfx: number;

    /** ACCESSOR_T(..., "movetype", movetype, GETN, SETINT) */
    movetype: number;

    /** ACCESSOR_T(..., "body", body, GETN, SETINT) */
    body: number;

    /** ACCESSOR_T(..., "aiment", aiment, GETN, SETINT) */
    aiment: number;

    /** ACCESSOR_T(..., "owner", owner, GETN, SETINT) */
    owner: number;

    /** ACCESSOR_T(..., "team", team, GETN, SETINT) */
    team: number;

    /** ACCESSOR_T(..., "playerclass", playerclass, GETN, SETINT) */
    playerclass: number;

    /** ACCESSOR_T(..., "health", health, GETN, SETINT) */
    health: number;

    /** ACCESSOR_T(..., "spectator", spectator, GETN, SETINT) */
    spectator: number;

    /** ACCESSOR_T(..., "weaponmodel", weaponmodel, GETN, SETINT) */
    weaponmodel: number;

    /** ACCESSOR_T(..., "gaitsequence", gaitsequence, GETN, SETINT) */
    gaitsequence: number;

    /** ACCESSOR_T(..., "usehull", usehull, GETN, SETINT) */
    usehull: number;

    /** ACCESSOR_T(..., "oldbuttons", oldbuttons, GETN, SETINT) */
    oldbuttons: number;

    /** ACCESSOR_T(..., "onground", onground, GETN, SETINT) */
    onground: number;

    /** ACCESSOR_T(..., "iStepLeft", iStepLeft, GETN, SETINT) */
    iStepLeft: number;

    /** ACCESSOR_T(..., "weaponanim", weaponanim, GETN, SETINT) */
    weaponanim: number;

    /** ACCESSOR_T(..., "msg_time", msg_time, GETN, SETFLOAT) */
    msg_time: number;

    /** ACCESSOR_T(..., "frame", frame, GETN, SETFLOAT) */
    frame: number;

    /** ACCESSOR_T(..., "scale", scale, GETN, SETFLOAT) */
    scale: number;

    /** ACCESSOR_T(..., "animtime", animtime, GETN, SETFLOAT) */
    animtime: number;

    /** ACCESSOR_T(..., "framerate", framerate, GETN, SETFLOAT) */
    framerate: number;

    /** ACCESSOR_T(..., "friction", friction, GETN, SETFLOAT) */
    friction: number;

    /** ACCESSOR_T(..., "gravity", gravity, GETN, SETFLOAT) */
    gravity: number;

    /** ACCESSOR_T(..., "flFallVelocity", flFallVelocity, GETN, SETFLOAT) */
    flFallVelocity: number;

    /** ACCESSOR_T(..., "fov", fov, GETN, SETFLOAT) */
    fov: number;

    /** ACCESSOR_T(..., "origin", origin, GETVEC3, SETVEC3) */
    origin: number[];

    /** ACCESSOR_T(..., "angles", angles, GETVEC3, SETVEC3) */
    angles: number[];

    /** ACCESSOR_T(..., "velocity", velocity, GETVEC3, SETVEC3) */
    velocity: number[];

    /** ACCESSOR_T(..., "mins", mins, GETVEC3, SETVEC3) */
    mins: number[];

    /** ACCESSOR_T(..., "maxs", maxs, GETVEC3, SETVEC3) */
    maxs: number[];

    /** ACCESSOR_T(..., "basevelocity", basevelocity, GETVEC3, SETVEC3) */
    basevelocity: number[];

    /** ACCESSOR_T(..., "startpos", startpos, GETVEC3, SETVEC3) */
    startpos: number[];

    /** ACCESSOR_T(..., "endpos", endpos, GETVEC3, SETVEC3) */
    endpos: number[];

    /** ACCESSOR_T(..., "iuser1", iuser1, GETN, SETINT) */
    iuser1: number;

    /** ACCESSOR_T(..., "iuser2", iuser2, GETN, SETINT) */
    iuser2: number;

    /** ACCESSOR_T(..., "iuser3", iuser3, GETN, SETINT) */
    iuser3: number;

    /** ACCESSOR_T(..., "iuser4", iuser4, GETN, SETINT) */
    iuser4: number;

    /** ACCESSOR_T(..., "fuser1", fuser1, GETN, SETFLOAT) */
    fuser1: number;

    /** ACCESSOR_T(..., "fuser2", fuser2, GETN, SETFLOAT) */
    fuser2: number;

    /** ACCESSOR_T(..., "fuser3", fuser3, GETN, SETFLOAT) */
    fuser3: number;

    /** ACCESSOR_T(..., "fuser4", fuser4, GETN, SETFLOAT) */
    fuser4: number;

    /** ACCESSOR_T(..., "vuser1", vuser1, GETVEC3, SETVEC3) */
    vuser1: number[];

    /** ACCESSOR_T(..., "vuser2", vuser2, GETVEC3, SETVEC3) */
    vuser2: number[];

    /** ACCESSOR_T(..., "vuser3", vuser3, GETVEC3, SETVEC3) */
    vuser3: number[];

    /** ACCESSOR_T(..., "vuser4", vuser4, GETVEC3, SETVEC3) */
    vuser4: number[];

    /** templ->SetNativeDataProperty("rendercolor", ...) */
    rendercolor: Entity | null;

    /** templ->SetNativeDataProperty("controller", ...) */
    controller: Entity | null;

    /** templ->SetNativeDataProperty("blending", ...) */
    blending: Entity | null;
  }
  /** Player input commands sent from client to server */
  interface UserCmd {
    /** ACCESSOR_T(..., "lerp_msec", lerp_msec, GETN, SETINT) */
    lerp_msec: number;

    /** ACCESSOR_T(..., "msec", msec, GETN, SETINT) */
    msec: number;

    /** ACCESSOR_T(..., "lightlevel", lightlevel, GETN, SETINT) */
    lightlevel: number;

    /** ACCESSOR_T(..., "buttons", buttons, GETN, SETINT) */
    buttons: number;

    /** ACCESSOR_T(..., "impulse", impulse, GETN, SETINT) */
    impulse: number;

    /** ACCESSOR_T(..., "weaponselect", weaponselect, GETN, SETINT) */
    weaponselect: number;

    /** ACCESSOR_T(..., "impact_index", impact_index, GETN, SETINT) */
    impact_index: number;

    /** ACCESSOR_T(..., "forwardmove", forwardmove, GETN, SETFLOAT) */
    forwardmove: number;

    /** ACCESSOR_T(..., "sidemove", sidemove, GETN, SETFLOAT) */
    sidemove: number;

    /** ACCESSOR_T(..., "upmove", upmove, GETN, SETFLOAT) */
    upmove: number;

    /** ACCESSOR_T(..., "viewangles", viewangles, GETVEC3, SETVEC3) */
    viewangles: number[];

    /** ACCESSOR_T(..., "impact_position", impact_position, GETVEC3, SETVEC3) */
    impact_position: number[];
  }
  /** Network address information */
  interface NetAdr {
    /** ACCESSOR_T(..., "port", port, GETN, SETINT) */
    port: number;

    /** templ->SetNativeDataProperty("type", ...) */
    type: string;

    /** templ->SetNativeDataProperty("ip", ...) */
    ip: number[];

    /** templ->SetNativeDataProperty("ipx", ...) */
    ipx: number[];

    /** templ->SetNativeDataProperty("ipString", ...) */
    ipString: string;
  }
  /** Weapon state and timing information */
  interface WeaponData {
    /** ACCESSOR_T(..., "m_iId", m_iId, GETN, SETINT) */
    m_iId: number;

    /** ACCESSOR_T(..., "m_iClip", m_iClip, GETN, SETINT) */
    m_iClip: number;

    /** ACCESSOR_T(..., "m_fInReload", m_fInReload, GETN, SETINT) */
    m_fInReload: number;

    /** ACCESSOR_T(..., "m_fInSpecialReload", m_fInSpecialReload, GETN, SETINT) */
    m_fInSpecialReload: number;

    /** ACCESSOR_T(..., "m_fInZoom", m_fInZoom, GETN, SETINT) */
    m_fInZoom: number;

    /** ACCESSOR_T(..., "m_iWeaponState", m_iWeaponState, GETN, SETINT) */
    m_iWeaponState: number;

    /** ACCESSOR_T(..., "m_flNextPrimaryAttack", m_flNextPrimaryAttack, GETN, SETFLOAT) */
    m_flNextPrimaryAttack: number;

    /** ACCESSOR_T(..., "m_flNextSecondaryAttack", m_flNextSecondaryAttack, GETN, SETFLOAT) */
    m_flNextSecondaryAttack: number;

    /** ACCESSOR_T(..., "m_flTimeWeaponIdle", m_flTimeWeaponIdle, GETN, SETFLOAT) */
    m_flTimeWeaponIdle: number;

    /** ACCESSOR_T(..., "m_flNextReload", m_flNextReload, GETN, SETFLOAT) */
    m_flNextReload: number;

    /** ACCESSOR_T(..., "m_flPumpTime", m_flPumpTime, GETN, SETFLOAT) */
    m_flPumpTime: number;

    /** ACCESSOR_T(..., "m_fReloadTime", m_fReloadTime, GETN, SETFLOAT) */
    m_fReloadTime: number;

    /** ACCESSOR_T(..., "m_fAimedDamage", m_fAimedDamage, GETN, SETFLOAT) */
    m_fAimedDamage: number;

    /** ACCESSOR_T(..., "m_fNextAimBonus", m_fNextAimBonus, GETN, SETFLOAT) */
    m_fNextAimBonus: number;

    /** ACCESSOR_T(..., "iuser1", iuser1, GETN, SETINT) */
    iuser1: number;

    /** ACCESSOR_T(..., "iuser2", iuser2, GETN, SETINT) */
    iuser2: number;

    /** ACCESSOR_T(..., "iuser3", iuser3, GETN, SETINT) */
    iuser3: number;

    /** ACCESSOR_T(..., "iuser4", iuser4, GETN, SETINT) */
    iuser4: number;

    /** ACCESSOR_T(..., "fuser1", fuser1, GETN, SETFLOAT) */
    fuser1: number;

    /** ACCESSOR_T(..., "fuser2", fuser2, GETN, SETFLOAT) */
    fuser2: number;

    /** ACCESSOR_T(..., "fuser3", fuser3, GETN, SETFLOAT) */
    fuser3: number;

    /** ACCESSOR_T(..., "fuser4", fuser4, GETN, SETFLOAT) */
    fuser4: number;
  }
  /** Player movement state and physics parameters */
  interface PlayerMove {
    /** ACCESSOR_T(..., "player_index", player_index, GETN, SETINT) */
    player_index: number;

    /** ACCESSOR_T(..., "server", server, GETBOOL, SETBOOL) */
    server: boolean;

    /** ACCESSOR_T(..., "multiplayer", multiplayer, GETBOOL, SETBOOL) */
    multiplayer: boolean;

    /** ACCESSOR_T(..., "flags", flags, GETN, SETINT) */
    flags: number;

    /** ACCESSOR_T(..., "usehull", usehull, GETN, SETINT) */
    usehull: number;

    /** ACCESSOR_T(..., "gravity", gravity, GETN, SETFLOAT) */
    gravity: number;

    /** ACCESSOR_T(..., "maxspeed", maxspeed, GETN, SETFLOAT) */
    maxspeed: number;

    /** ACCESSOR_T(..., "clientmaxspeed", clientmaxspeed, GETN, SETFLOAT) */
    clientmaxspeed: number;

    /** ACCESSOR_T(..., "movetype", movetype, GETN, SETINT) */
    movetype: number;

    /** ACCESSOR_T(..., "friction", friction, GETN, SETFLOAT) */
    friction: number;

    /** ACCESSOR_T(..., "oldbuttons", oldbuttons, GETN, SETINT) */
    oldbuttons: number;

    /** ACCESSOR_T(..., "onground", onground, GETN, SETINT) */
    onground: number;

    /** ACCESSOR_T(..., "waterlevel", waterlevel, GETN, SETINT) */
    waterlevel: number;

    /** ACCESSOR_T(..., "watertype", watertype, GETN, SETINT) */
    watertype: number;

    /** ACCESSOR_T(..., "oldwaterlevel", oldwaterlevel, GETN, SETINT) */
    oldwaterlevel: number;

    /** ACCESSOR_T(..., "chtexturetype", chtexturetype, GETN, SETINT) */
    chtexturetype: number;

    /** ACCESSOR_T(..., "dead", dead, GETN, SETINT) */
    dead: number;

    /** ACCESSOR_T(..., "deadflag", deadflag, GETN, SETINT) */
    deadflag: number;

    /** ACCESSOR_T(..., "spectator", spectator, GETN, SETINT) */
    spectator: number;

    /** ACCESSOR_T(..., "bInDuck", bInDuck, GETN, SETINT) */
    bInDuck: number;

    /** ACCESSOR_T(..., "flTimeStepSound", flTimeStepSound, GETN, SETINT) */
    flTimeStepSound: number;

    /** ACCESSOR_T(..., "iStepLeft", iStepLeft, GETN, SETINT) */
    iStepLeft: number;

    /** ACCESSOR_T(..., "numphysent", numphysent, GETN, SETINT) */
    numphysent: number;

    /** ACCESSOR_T(..., "nummoveent", nummoveent, GETN, SETINT) */
    nummoveent: number;

    /** ACCESSOR_T(..., "numvisent", numvisent, GETN, SETINT) */
    numvisent: number;

    /** ACCESSOR_T(..., "numtouch", numtouch, GETN, SETINT) */
    numtouch: number;

    /** ACCESSOR_T(..., "runfuncs", runfuncs, GETBOOL, SETBOOL) */
    runfuncs: boolean;

    /** ACCESSOR_T(..., "time", time, GETN, SETFLOAT) */
    time: number;

    /** ACCESSOR_T(..., "frametime", frametime, GETN, SETFLOAT) */
    frametime: number;

    /** ACCESSOR_T(..., "flFallVelocity", flFallVelocity, GETN, SETFLOAT) */
    flFallVelocity: number;

    /** ACCESSOR_T(..., "flSwimTime", flSwimTime, GETN, SETFLOAT) */
    flSwimTime: number;

    /** ACCESSOR_T(..., "flDuckTime", flDuckTime, GETN, SETFLOAT) */
    flDuckTime: number;

    /** ACCESSOR_T(..., "fuser1", fuser1, GETN, SETFLOAT) */
    fuser1: number;

    /** ACCESSOR_T(..., "fuser2", fuser2, GETN, SETFLOAT) */
    fuser2: number;

    /** ACCESSOR_T(..., "fuser3", fuser3, GETN, SETFLOAT) */
    fuser3: number;

    /** ACCESSOR_T(..., "fuser4", fuser4, GETN, SETFLOAT) */
    fuser4: number;

    /** ACCESSOR_T(..., "forward", forward, GETVEC3, SETVEC3) */
    forward: number[];

    /** ACCESSOR_T(..., "right", right, GETVEC3, SETVEC3) */
    right: number[];

    /** ACCESSOR_T(..., "up", up, GETVEC3, SETVEC3) */
    up: number[];

    /** ACCESSOR_T(..., "origin", origin, GETVEC3, SETVEC3) */
    origin: number[];

    /** ACCESSOR_T(..., "angles", angles, GETVEC3, SETVEC3) */
    angles: number[];

    /** ACCESSOR_T(..., "oldangles", oldangles, GETVEC3, SETVEC3) */
    oldangles: number[];

    /** ACCESSOR_T(..., "velocity", velocity, GETVEC3, SETVEC3) */
    velocity: number[];

    /** ACCESSOR_T(..., "movedir", movedir, GETVEC3, SETVEC3) */
    movedir: number[];

    /** ACCESSOR_T(..., "basevelocity", basevelocity, GETVEC3, SETVEC3) */
    basevelocity: number[];

    /** ACCESSOR_T(..., "view_ofs", view_ofs, GETVEC3, SETVEC3) */
    view_ofs: number[];

    /** ACCESSOR_T(..., "punchangle", punchangle, GETVEC3, SETVEC3) */
    punchangle: number[];

    /** ACCESSOR_T(..., "vuser1", vuser1, GETVEC3, SETVEC3) */
    vuser1: number[];

    /** ACCESSOR_T(..., "vuser2", vuser2, GETVEC3, SETVEC3) */
    vuser2: number[];

    /** ACCESSOR_T(..., "vuser3", vuser3, GETVEC3, SETVEC3) */
    vuser3: number[];

    /** ACCESSOR_T(..., "vuser4", vuser4, GETVEC3, SETVEC3) */
    vuser4: number[];

    /** ACCESSOR_T(..., "iuser1", iuser1, GETN, SETINT) */
    iuser1: number;

    /** ACCESSOR_T(..., "iuser2", iuser2, GETN, SETINT) */
    iuser2: number;

    /** ACCESSOR_T(..., "iuser3", iuser3, GETN, SETINT) */
    iuser3: number;

    /** ACCESSOR_T(..., "iuser4", iuser4, GETN, SETINT) */
    iuser4: number;

    /** templ->SetNativeDataProperty("sztexturename", ...) */
    sztexturename: string;

    /** templ->SetNativeDataProperty("physinfo", ...) */
    physinfo: string;

    /** templ->SetNativeDataProperty("cmd", ...) */
    cmd: UserCmd | null;

    /** templ->SetNativeDataProperty("movevars", ...) */
    movevars: object | null;
  }
  /** Player customization data (sprays, models) */
  interface Customization {
    /** ACCESSOR_T(..., "bInUse", bInUse, GETBOOL, SETBOOL) */
    bInUse: boolean;

    /** ACCESSOR_T(..., "bTranslated", bTranslated, GETBOOL, SETBOOL) */
    bTranslated: boolean;

    /** ACCESSOR_T(..., "nUserData1", nUserData1, GETN, SETINT) */
    nUserData1: number;

    /** ACCESSOR_T(..., "nUserData2", nUserData2, GETN, SETINT) */
    nUserData2: number;

    /** templ->SetNativeDataProperty("resource", ...) */
    resource: object;

    /** templ->SetNativeDataProperty("pInfo", ...) */
    pInfo: null;

    /** templ->SetNativeDataProperty("pBuffer", ...) */
    pBuffer: null;

    /** templ->SetNativeDataProperty("pNext", ...) */
    pNext: Customization | null;
  }
  /** Key-value pairs for entity spawning */
  interface KeyValueData {
    /** ACCESSOR_T(..., "fHandled", fHandled, GETN, SETINT) */
    fHandled: number;

    /** templ->SetNativeDataProperty("szClassName", ...) */
    szClassName: string;

    /** templ->SetNativeDataProperty("szKeyName", ...) */
    szKeyName: string;

    /** templ->SetNativeDataProperty("szValue", ...) */
    szValue: string;
  }
  /** Save/restore game state information */
  interface SaveRestoreData {
    /** ACCESSOR_T(..., "size", size, GETN, SETINT) */
    size: number;

    /** ACCESSOR_T(..., "bufferSize", bufferSize, GETN, SETINT) */
    bufferSize: number;

    /** ACCESSOR_T(..., "tokenSize", tokenSize, GETN, SETINT) */
    tokenSize: number;

    /** ACCESSOR_T(..., "tokenCount", tokenCount, GETN, SETINT) */
    tokenCount: number;

    /** templ->SetNativeDataProperty("szCurrentMapName", ...) */
    szCurrentMapName: string;
  }
  /** Field type description for save/restore system */
  interface TypeDescription {
    /** ACCESSOR_T(..., "fieldOffset", fieldOffset, GETN, SETINT) */
    fieldOffset: number;

    /** ACCESSOR_T(..., "fieldSize", fieldSize, GETN, SETINT) */
    fieldSize: number;

    /** ACCESSOR_T(..., "flags", flags, GETN, SETINT) */
    flags: number;

    /** templ->SetNativeDataProperty("fieldType", ...) */
    fieldType: number;

    /** templ->SetNativeDataProperty("fieldName", ...) */
    fieldName: string;

    /** templ->SetNativeDataProperty("fieldTypeName", ...) */
    fieldTypeName: string;

    /** templ->SetNativeDataProperty("isGlobal", ...) */
    isGlobal: boolean;
  }
  /** Delta compression structure for network optimization */
  interface Delta {
    /** templ->SetNativeDataProperty("toString", ...) */
    toString: string;

    /** templ->SetNativeDataProperty("isValid", ...) */
    isValid: boolean;

    /** templ->SetNativeDataProperty("pointer", ...) */
    pointer: number;
  }
  /** Console variable (cvar) information */
  interface Cvar {
    /** ACCESSOR_T(..., "flags", flags, GETN, SETINT) */
    flags: number;

    /** ACCESSOR_T(..., "value", value, GETN, SETFLOAT) */
    value: number;

    /** templ->SetNativeDataProperty("name", ...) */
    name: string;

    /** templ->SetNativeDataProperty("string", ...) */
    string: string;

    /** templ->SetNativeDataProperty("next", ...) */
    next: Cvar | null;
  }
  /** Results from line/hull trace operations */
  interface TraceResult {
    /** v8::String::NewFromUtf8(isolate, "fraction").ToLocalChecked(), v8::Number::New(isolate, trace->flFraction)) */
    fraction: number;

    /** v8::String::NewFromUtf8(isolate, "planeDist").ToLocalChecked(), v8::Number::New(isolate, trace->flPlaneDist)) */
    planeDist: number;

    /** v8::String::NewFromUtf8(isolate, "endPos").ToLocalChecked(), endPos) */
    endPos: unknown;

    /** v8::String::NewFromUtf8(isolate, "planeNormal").ToLocalChecked(), planeNormal) */
    planeNormal: unknown;

    /** v8::String::NewFromUtf8(isolate, "hitGroup").ToLocalChecked(), v8::Integer::New(isolate, trace->iHitgroup)) */
    hitGroup: unknown;

    /** v8::String::NewFromUtf8(isolate, "allSolid").ToLocalChecked(), v8::Boolean::New(isolate, trace->fAllSolid)) */
    allSolid: boolean;

    /** v8::String::NewFromUtf8(isolate, "startSolid").ToLocalChecked(), v8::Boolean::New(isolate, trace->fStartSolid)) */
    startSolid: boolean;

    /** v8::String::NewFromUtf8(isolate, "inOpen").ToLocalChecked(), v8::Boolean::New(isolate, trace->fInOpen)) */
    inOpen: boolean;

    /** v8::String::NewFromUtf8(isolate, "inWater").ToLocalChecked(), v8::Boolean::New(isolate, trace->fInWater)) */
    inWater: boolean;

    /** v8::String::NewFromUtf8(isolate, "hit").ToLocalChecked(), v8::Null(isolate)) */
    hit: null;
  }
  /** Game entity reference with properties and methods */
  interface Entity {
    /** obj->Set(..., v8::Number::New(isolate, entityId)) */
    id: number;

    /** ACCESSOR(_entity, "classname", v.classname, GETSTR, SETSTR) */
    classname: string;

    /** ACCESSOR(_entity, "globalname", v.globalname, GETSTR, SETSTR) */
    globalname: string;

    /** ACCESSORL(_entity, "origin", v.origin, GETVEC3, SETVEC3) */
    origin: number[];

    /** ACCESSORL(_entity, "oldorigin", v.oldorigin, GETVEC3, SETVEC3) */
    oldorigin: number[];

    /** ACCESSORL(_entity, "velocity", v.velocity, GETVEC3, SETVEC3) */
    velocity: number[];

    /** ACCESSORL(_entity, "basevelocity", v.basevelocity, GETVEC3, SETVEC3) */
    basevelocity: number[];

    /** ACCESSORL(_entity, "clbasevelocity", v.clbasevelocity, GETVEC3, SETVEC3) */
    clbasevelocity: number[];

    /** ACCESSORL(_entity, "movedir", v.movedir, GETVEC3, SETVEC3) */
    movedir: number[];

    /** ACCESSORL(_entity, "angles", v.angles, GETVEC3, SETVEC3) */
    angles: number[];

    /** ACCESSORL(_entity, "avelocity", v.avelocity, GETVEC3, SETVEC3) */
    avelocity: number[];

    /** ACCESSORL(_entity, "punchangle", v.punchangle, GETVEC3, SETVEC3) */
    punchangle: number[];

    /** ACCESSORL(_entity, "angle", v.v_angle, GETVEC3, SETVEC3) */
    angle: number[];

    /** ACCESSORL(_entity, "endpos", v.endpos, GETVEC3, SETVEC3) */
    endpos: number[];

    /** ACCESSORL(_entity, "startpos", v.startpos, GETVEC3, SETVEC3) */
    startpos: number[];

    /** ACCESSOR(_entity, "impacttime", v.impacttime, GETN, SETFLOAT) */
    impacttime: number;

    /** ACCESSOR(_entity, "starttime", v.starttime, GETN, SETFLOAT) */
    starttime: number;

    /** ACCESSOR(_entity, "fixangle", v.fixangle, GETN, SETINT) */
    fixangle: number;

    /** ACCESSOR(_entity, "idealpitch", v.idealpitch, GETN, SETFLOAT) */
    idealpitch: number;

    /** ACCESSOR(_entity, "pitchSpeed", v.pitch_speed, GETN, SETFLOAT) */
    pitchSpeed: number;

    /** ACCESSOR(_entity, "idealYaw", v.ideal_yaw, GETN, SETFLOAT) */
    idealYaw: number;

    /** ACCESSOR(_entity, "yawSpeed", v.yaw_speed, GETN, SETFLOAT) */
    yawSpeed: number;

    /** ACCESSOR(_entity, "modelindex", v.modelindex, GETN, SETINT) */
    modelindex: number;

    /** _entity->SetNativeDataProperty(..., GETTER(v.model, GETSTR), ...) */
    model: string;

    /** ACCESSOR(_entity, "viewmodel", v.viewmodel, GETN, SETINT) */
    viewmodel: number;

    /** ACCESSOR(_entity, "weaponmodel", v.weaponmodel, GETN, SETINT) */
    weaponmodel: number;

    /** ACCESSORL(_entity, "absmin", v.absmin, GETVEC3, SETVEC3) */
    absmin: number[];

    /** ACCESSORL(_entity, "absmax", v.absmax, GETVEC3, SETVEC3) */
    absmax: number[];

    /** ACCESSORL(_entity, "mins", v.mins, GETVEC3, SETVEC3) */
    mins: number[];

    /** ACCESSORL(_entity, "maxs", v.maxs, GETVEC3, SETVEC3) */
    maxs: number[];

    /** ACCESSORL(_entity, "size", v.maxs, GETVEC3, SETVEC3) */
    size: number[];

    /** ACCESSOR(_entity, "ltime", v.ltime, GETN, SETFLOAT) */
    ltime: number;

    /** ACCESSOR(_entity, "nextthink", v.nextthink, GETN, SETFLOAT) */
    nextthink: number;

    /** ACCESSOR(_entity, "movetype", v.movetype, GETN, SETINT) */
    movetype: number;

    /** ACCESSOR(_entity, "solid", v.solid, GETN, SETINT) */
    solid: number;

    /** ACCESSOR(_entity, "skin", v.skin, GETN, SETINT) */
    skin: number;

    /** ACCESSOR(_entity, "body", v.body, GETN, SETINT) */
    body: number;

    /** ACCESSOR(_entity, "effects", v.effects, GETN, SETINT) */
    effects: number;

    /** ACCESSOR(_entity, "gravity", v.gravity, GETN, SETFLOAT) */
    gravity: number;

    /** ACCESSOR(_entity, "friction", v.friction, GETN, SETFLOAT) */
    friction: number;

    /** ACCESSOR(_entity, "lightLevel", v.light_level, GETN, SETINT) */
    lightLevel: number;

    /** ACCESSOR(_entity, "sequence", v.sequence, GETN, SETINT) */
    sequence: number;

    /** ACCESSOR(_entity, "gaitsequence", v.gaitsequence, GETN, SETINT) */
    gaitsequence: number;

    /** ACCESSOR(_entity, "frame", v.frame, GETN, SETFLOAT) */
    frame: number;

    /** ACCESSOR(_entity, "animtime", v.animtime, GETN, SETFLOAT) */
    animtime: number;

    /** ACCESSOR(_entity, "framerate", v.framerate, GETN, SETFLOAT) */
    framerate: number;

    /** ACCESSOR(_entity, "scale", v.scale, GETN, SETFLOAT) */
    scale: number;

    /** ACCESSOR(_entity, "rendermode", v.rendermode, GETN, SETINT) */
    rendermode: number;

    /** ACCESSOR(_entity, "renderamt", v.renderamt, GETN, SETFLOAT) */
    renderamt: number;

    /** ACCESSORL(_entity, "rendercolor", v.rendercolor, GETVEC3, SETVEC3) */
    rendercolor: number[];

    /** ACCESSOR(_entity, "renderfx", v.renderfx, GETN, SETINT) */
    renderfx: number;

    /** ACCESSOR(_entity, "health", v.health, GETN, SETFLOAT) */
    health: number;

    /** ACCESSOR(_entity, "frags", v.frags, GETN, SETFLOAT) */
    frags: number;

    /** ACCESSOR(_entity, "weapons", v.weapons, GETN, SETINT) */
    weapons: number;

    /** ACCESSOR(_entity, "takedamage", v.takedamage, GETN, SETFLOAT) */
    takedamage: number;

    /** ACCESSOR(_entity, "deadflag", v.deadflag, GETN, SETINT) */
    deadflag: number;

    /** ACCESSORL(_entity, "viewOfs", v.view_ofs, GETVEC3, SETVEC3) */
    viewOfs: number[];

    /** ACCESSOR(_entity, "button", v.button, GETN, SETINT) */
    button: number;

    /** ACCESSOR(_entity, "impulse", v.impulse, GETN, SETINT) */
    impulse: number;

    /** ACCESSOR(_entity, "spawnflags", v.spawnflags, GETN, SETINT) */
    spawnflags: number;

    /** ACCESSOR(_entity, "flags", v.flags, GETN, SETINT) */
    flags: number;

    /** ACCESSOR(_entity, "colormap", v.colormap, GETN, SETINT) */
    colormap: number;

    /** ACCESSOR(_entity, "team", v.team, GETN, SETINT) */
    team: number;

    /** ACCESSOR(_entity, "maxHealth", v.max_health, GETN, SETFLOAT) */
    maxHealth: number;

    /** ACCESSOR(_entity, "teleportTime", v.teleport_time, GETN, SETFLOAT) */
    teleportTime: number;

    /** ACCESSOR(_entity, "armortype", v.armortype, GETN, SETFLOAT) */
    armortype: number;

    /** ACCESSOR(_entity, "armorvalue", v.armorvalue, GETN, SETFLOAT) */
    armorvalue: number;

    /** ACCESSOR(_entity, "waterlevel", v.waterlevel, GETN, SETINT) */
    waterlevel: number;

    /** ACCESSOR(_entity, "watertype", v.watertype, GETN, SETINT) */
    watertype: number;

    /** ACCESSOR(_entity, "target", v.target, GETSTR, SETSTR) */
    target: string;

    /** ACCESSOR(_entity, "targetname", v.targetname, GETSTR, SETSTR) */
    targetname: string;

    /** ACCESSOR(_entity, "netname", v.netname, GETSTR, SETSTR) */
    netname: string;

    /** ACCESSOR(_entity, "message", v.message, GETSTR, SETSTR) */
    message: string;

    /** ACCESSOR(_entity, "dmgTake", v.dmg_take, GETN, SETFLOAT) */
    dmgTake: number;

    /** ACCESSOR(_entity, "dmgSave", v.dmg_save, GETN, SETFLOAT) */
    dmgSave: number;

    /** ACCESSOR(_entity, "dmg", v.dmg, GETN, SETFLOAT) */
    dmg: number;

    /** ACCESSOR(_entity, "dmgtime", v.dmgtime, GETN, SETFLOAT) */
    dmgtime: number;

    /** ACCESSOR(_entity, "noise", v.noise, GETSTR, SETSTR) */
    noise: string;

    /** ACCESSOR(_entity, "noise1", v.noise1, GETSTR, SETSTR) */
    noise1: string;

    /** ACCESSOR(_entity, "noise2", v.noise2, GETSTR, SETSTR) */
    noise2: string;

    /** ACCESSOR(_entity, "noise3", v.noise3, GETSTR, SETSTR) */
    noise3: string;

    /** ACCESSOR(_entity, "speed", v.speed, GETN, SETFLOAT) */
    speed: number;

    /** ACCESSOR(_entity, "airFinished", v.air_finished, GETN, SETFLOAT) */
    airFinished: number;

    /** ACCESSOR(_entity, "painFinished", v.pain_finished, GETN, SETFLOAT) */
    painFinished: number;

    /** ACCESSOR(_entity, "radsuitFinished", v.radsuit_finished, GETN, SETFLOAT) */
    radsuitFinished: number;

    /** ACCESSOR(_entity, "playerclass", v.playerclass, GETN, SETINT) */
    playerclass: number;

    /** ACCESSOR(_entity, "maxspeed", v.maxspeed, GETN, SETFLOAT) */
    maxspeed: number;

    /** ACCESSOR(_entity, "fov", v.fov, GETN, SETFLOAT) */
    fov: number;

    /** ACCESSOR(_entity, "weaponanim", v.weaponanim, GETN, SETINT) */
    weaponanim: number;

    /** ACCESSOR(_entity, "pushmsec", v.pushmsec, GETN, SETINT) */
    pushmsec: number;

    /** ACCESSOR(_entity, "bInDuck", v.bInDuck, GETN, SETINT) */
    bInDuck: number;

    /** ACCESSOR(_entity, "flTimeStepSound", v.flTimeStepSound, GETN, SETINT) */
    flTimeStepSound: number;

    /** ACCESSOR(_entity, "flSwimTime", v.flSwimTime, GETN, SETINT) */
    flSwimTime: number;

    /** ACCESSOR(_entity, "flDuckTime", v.flDuckTime, GETN, SETINT) */
    flDuckTime: number;

    /** ACCESSOR(_entity, "iStepLeft", v.iStepLeft, GETN, SETINT) */
    iStepLeft: number;

    /** ACCESSOR(_entity, "fallVelocity", v.flFallVelocity, GETN, SETFLOAT) */
    fallVelocity: number;

    /** ACCESSOR(_entity, "gamestate", v.gamestate, GETN, SETINT) */
    gamestate: number;

    /** ACCESSOR(_entity, "oldbuttons", v.oldbuttons, GETN, SETINT) */
    oldbuttons: number;

    /** ACCESSOR(_entity, "groupinfo", v.groupinfo, GETN, SETINT) */
    groupinfo: number;
  }
  /** Result from monster hull trace operations */
  interface TraceMonsterHullResult {
    /** Return value from pfnTraceMonsterHull - likely collision/movement validity flag */
    result: number;

    /** Standard trace result with hit information */
    trace: TraceResult;
  }
}