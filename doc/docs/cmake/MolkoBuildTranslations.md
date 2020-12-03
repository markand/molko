# molko_build_translations

Build translations and update them.

## Synopsis

```cmake
molko_build_translations(
TARGET              target name
SOURCES             target sources
OUTPUTS             output variable
TRANSLATIONS        list of localizations
)
```

Generate target and output commands for NLS (via GNU gettext) support for the
given *TARGET* name.

The argument *SOURCES* must contain sources to extract gettext keywords, it
will search for _, N_. The list of *SOURCES* can contain any files, only .c
and .h will be filtered.

The argument *OUTPUTS* will be set with the generated .mo files in the binary
directory and installed to *CMAKE_INSTALL_LOCALEDIR*.

The argument *TRANSLATIONS* should contain a list of languages supported in the
gettext form (ll_LL@variant, see ISO 639 and ISO 3166 for more details).

This macro create a `<TARGET>-po` target that will recreate the .pot file and
every .po files in the nls/ directory for each language specified in
*TRANSLATIONS*. Note, if you add a new language into translations but do not
copy the .pot file, a warning will be issued and you should copy the .pot
file as the new .po language file.

Since the target is modifying files directly in the source tree they are not
included in any build process and must be invoked manually.

