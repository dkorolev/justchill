#!/usr/bin/env node

// JavaScript is the cleanest way to format JSON w/o reordering the fields.
// Note: It would mess up int64 values that don't fit 53 bits. Oh well. There are none in our schema. -- D.K.

var fs = require('fs');

var fn = 'box.json';

fs.writeFileSync(fn, JSON.stringify(JSON.parse(fs.readFileSync(fn).toString()), null, 2))
