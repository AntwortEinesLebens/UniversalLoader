# SPDX-FileCopyrightText: 2025 The UniversalLoader development team
#
# SPDX-License-Identifier: CC0-1.0

project(
  'UniversalLoader',
  'c',
  default_options: [
    'b_coverage=false',
    'c_std=c23',
    'warning_level=3',
    'werror=true',
    'default_both_libraries=static',
    'default_library=static',
  ],
  license: 'GPL-3.0-or-later',
  license_files: './LICENSES/GPL-3.0-or-later.txt',
  meson_version: '>= 1.3.0',
  version: '0.0.1',
)

facil = subproject('facil', default_options: ['werror=false'])

subdir('./loader/')
subdir('./server/')
