/* -*- buffer-read-only: t -*- vi: set ro: */
/* DO NOT EDIT! GENERATED AUTOMATICALLY! */
#line 1
/* Test of getline() function.
   Copyright (C) 2007-2010 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Written by Eric Blake <ebb9@byu.net>, 2007.  */

#include <config.h>

#include <stdio.h>

#include "signature.h"
SIGNATURE_CHECK (getline, ssize_t, (char **, size_t *, FILE *));

#include <stdlib.h>
#include <string.h>

#include "macros.h"

int
main (void)
{
  FILE *f;
  char *line = NULL;
  size_t len = 0;
  ssize_t result;

  /* Create test file.  */
  f = fopen ("test-getline.txt", "wb");
  if (!f || fwrite ("a\nbc\nd\0f", 1, 8, f) != 8 || fclose (f) != 0)
    {
      fputs ("Failed to create sample file.\n", stderr);
      remove ("test-getline.txt");
      return 1;
    }
  f = fopen ("test-getline.txt", "rb");
  if (!f)
    {
      fputs ("Failed to reopen sample file.\n", stderr);
      remove ("test-getline.txt");
      return 1;
    }

  /* Test initial allocation, which must include trailing NUL.  */
  result = getline (&line, &len, f);
  ASSERT (result == 2);
  ASSERT (strcmp (line, "a\n") == 0);
  ASSERT (2 < len);

  /* Test growth of buffer, must not leak.  */
  free (line);
  line = malloc (1);
  len = 0;
  result = getline (&line, &len, f);
  ASSERT (result == 3);
  ASSERT (strcmp (line, "bc\n") == 0);
  ASSERT (3 < len);

  /* Test embedded NULs and EOF behavior.  */
  result = getline (&line, &len, f);
  ASSERT (result == 3);
  ASSERT (memcmp (line, "d\0f", 4) == 0);
  ASSERT (3 < len);

  result = getline (&line, &len, f);
  ASSERT (result == -1);

  free (line);
  fclose (f);
  remove ("test-getline.txt");
  return 0;
}
