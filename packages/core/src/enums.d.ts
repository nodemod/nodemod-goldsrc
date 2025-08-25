// This file is generated automatically. Don't edit it.
declare namespace nodemod {
  // Metamod result constants
  const enum META_RES {
    UNSET = 0,    // Uninitialized (causes error)
    IGNORED = 1,  // Plugin didn't take any action, continue normally
    HANDLED = 2,  // Plugin did something, but original function still executes
    OVERRIDE = 3, // Execute original function, but use plugin's return value
    SUPERCEDE = 4 // Skip original function entirely, use plugin's behavior
  }

  // Alert types for engine functions
  const enum ALERT_TYPE {
    at_notice = 0,
    at_console = 1,
    at_aiconsole = 2,
    at_warning = 3,
    at_error = 4,
    at_logged = 5
  }

  // Print types for client output
  const enum PRINT_TYPE {
    print_console = 0,
    print_center = 1,
    print_chat = 2
  }

  // Force types for consistency checking
  const enum FORCE_TYPE {
    force_exactfile = 0,
    force_model_samebounds = 1,
    force_model_specifybounds = 2,
    force_model_specifybounds_if_avail = 3
  }
}