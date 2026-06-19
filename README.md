# OPA Milking

OPA Milking adds a milking system to DayZ. Server owners can place static milkable cows in farms and rural points of interest without using animal AI.

The mod adds a new consumable liquid, Milk, and a static interactive cow class. Cows remain saved after server restart and become available for milking again according to the server configuration.

## Features

- Static interactive cow: `OPA_MilkingCow`.
- New consumable liquid: Milk.
- Continuous milking action.
- Partial milk reward if the action is interrupted.
- Persistent cow position and orientation.
- Milking reset after server restart.
- Configurable milk amount per milking.
- Configurable number of milkings per cow per restart.
- Compatible with vanilla and modded containers that support water.
- Prevents milk from mixing with other liquids.
- Custom milking sound.
- Random cow ambient sounds.

## Requirements

- DayZ Server.
- Same mod version on client and server.
- `OPAMilking.bikey` installed on the server if signature verification is enabled.

OPA Milking does not require CF, COT, VPPAdminTools, Expansion, or any other framework. Admin tools are only useful for spawning and positioning cows.

## Server Installation

Copy the mod folder to your DayZ server root:

```txt
DayZServer/
@OPAMilking/
  Addons/
    OPAMilking.pbo
    OPAMilking.pbo.OPAMilking.bisign
  mod.cpp
```

Copy the key to:

```txt
DayZServer/keys/OPAMilking.bikey
```

Add the mod to your server startup line:

```txt
-mod=@OPAMilking
```

Example with admin tools:

```txt
-mod=@CF;@Community-Online-Tools;@VPPAdminTools;@OPAMilking
```

Restart the server.

## Client Installation

Clients must load the same mod version as the server:

```txt
@OPAMilking
```

For public servers, publishing the mod on Steam Workshop is recommended.

## Cow Class

Use this class when spawning or mapping cows:

```txt
OPA_MilkingCow
```

You can place cows with:

- VPP Admin Tools;
- Community Online Tools;
- DayZ Editor;
- init.c;
- compatible mapping tools.

The cow is static and does not inherit from `AnimalBase`.

## Persistence

Cow positions are saved automatically in:

```txt
Profiles/OPAMilking/cows.json
```

The file stores:

- position;
- orientation.

The milking state is not saved. This is intentional. After every server restart, cows become available again according to the configured limit.

## Configuration

The server creates this file automatically:

```txt
Profiles/OPAMilking/settings.json
```

Default configuration:

```json
{
    "MilkAmountPerMilking": 1000.0,
    "MilkingsPerCowRestart": 1
}
```

Options:

- `MilkAmountPerMilking`: maximum milk amount generated per complete milking action, in ml.
- `MilkingsPerCowRestart`: how many times each cow can be milked per restart.

Example:

```json
{
    "MilkAmountPerMilking": 750.0,
    "MilkingsPerCowRestart": 2
}
```

Restart the server after changing this file.

## Milk Nutrition

Milk nutrition values are configured in `config.cpp`:

```cpp
#define OPA_MILK_FULLNESS_INDEX 1
#define OPA_MILK_ENERGY 500
#define OPA_MILK_WATER 100
#define OPA_MILK_NUTRITIONAL_INDEX 80
#define OPA_MILK_TOXICITY 0
#define OPA_MILK_DIGESTIBILITY 2
```

Changing these values requires rebuilding and resigning the PBO.

## Milking Rules

The action appears when:

- the player is aiming at an `OPA_MilkingCow`;
- the player is holding a valid liquid container;
- the container is empty or already contains Milk;
- the container has free space;
- the cow has not reached its milking limit for the current restart.

The action does not appear when:

- the container has water, gasoline, alcohol, disinfectant, or any other liquid;
- the container is full;
- the item is not a valid liquid container;
- the cow has reached its restart limit.

If the action is interrupted before completion, the container receives milk proportional to the time spent milking.

## Container Compatibility

OPA Milking avoids direct config overrides for specific vanilla containers.

Instead, it adds Milk support to any item whose liquid container mask already supports water. This improves compatibility with vanilla and modded containers.

Examples:

- water bottles;
- canteens;
- cooking pots;
- cauldrons;
- barrels;
- modded containers that support water.

Milk does not mix with other liquids. A container with Milk can receive more Milk, but a container with any other liquid blocks the milking action.

## Troubleshooting

### The milking action does not appear

Check:

- the item in hands is a liquid container;
- the container is empty or contains Milk;
- the container is not full;
- the target is `OPA_MilkingCow`;
- the cow still has available milkings for the current restart.

### Milk appears as ERROR

Possible causes:

- client is not loading the mod;
- client and server are using different versions;
- old PBO on client or server;
- another mod is overriding liquid UI or tooltips.

### Cow disappeared after restart

Check:

```txt
Profiles/OPAMilking/cows.json
```

If the file is empty, place the cow again and wait a few seconds before restarting the server.

### Signature kick

Check:

```txt
DayZServer/keys/OPAMilking.bikey
```

Also confirm that the client is using the same PBO version as the server.

## Important Files

Workshop/server package:

```txt
@OPAMilking/
  Addons/
    OPAMilking.pbo
    OPAMilking.pbo.OPAMilking.bisign
  mod.cpp
```

Server profile:

```txt
Profiles/OPAMilking/
  cows.json
  settings.json
```

## Repacking

This mod may be repacked into server modpacks as long as the original developer, Opaverick, is credited.

When repacking:

- keep the class name `OPA_MilkingCow`;
- preserve or correctly adjust the internal prefix `OPAMilking`;
- resign the PBO with your own key;
- never publish a private signing key;
- keep the MIT license notice.

Keeping the cow class name avoids breaking existing `cows.json` files and mapping data.

## License

OPA Milking is open source under the MIT License.

See:

```txt
LICENSE
REPACKING.md
```
