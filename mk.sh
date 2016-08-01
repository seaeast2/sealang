#!/bin/bash

rm tags
rm cscope.*
mkctags.sh
mkcscope.sh
