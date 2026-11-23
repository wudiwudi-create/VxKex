VxKex 1.2.2.2316 - Windows 7 SP1 x64 interrupt-time compatibility

This package contains a modified kxbase.dll, an export list, and a test program.
Use it only with a matching VxKex 1.2.2.2316 installation. This is an
experimental build; it has not been validated on your Windows 7 machine.

1. Close Antigravity and VxKex-enabled applications.
2. Open C:\Program Files\VxKex\Kex64 and back up the existing kxbase.dll.
3. Copy the supplied kxbase.dll to that folder, replacing the original.
4. Enable VxKex for interrupttime-test.exe, run it, and check for PASS.
5. Run agy.exe with VxKex enabled. If it fails, restore the backup and record
   the error. Passing the test does not guarantee Antigravity will start.

The precise time APIs are compatibility approximations using the Windows 7
system clock tick; they do not have Windows 10's high-resolution precision.
