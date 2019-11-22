#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2010, 2012 Peter Miller
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or (at
# you option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>
#

TEST_SUBJECT="ucsdpsys_opcodes"
. test_prelude

cat > opcodes.txt << 'fubar'
  0 = two;
  1 = short;
  2 = lopt;
  3 = blk;
  4 = cmprss2;
  5 = opt;
  6 = lopt;
  7 = cmprss2;
  8 = opt;
  9 = chrs;
 10 = one;
 11 = chrs;
 12 = two;
 13 = two;
 14 = opt;
 15 = cmprss;
 16 = opt;
 17 = chrs;
 18 = cmprss;
 19 = chrs;
 20 = two;
 21 = lopt;
 22 = words;
 23 = words;
 24 = words;
 25 = blk;
 26 = blk;
 27 = words;
 28 = one;
 29 = two;
 30 = word;
 31 = lopt;
 32 = two;
 33 = two;
 34 = cmprss2;
 35 = short;
 36 = lopt;
 37 = opt;
 38 = cmprss2;
 39 = chrs;
 40 = cmprss2;
 41 = cmprss2;
 42 = word;
 43 = word;
 44 = one;
 45 = one;
 46 = words;
 47 = opt;
 48 = chrs;
 49 = opt;
 50 = chrs;
 51 = cmprss2;
 52 = words,   "ROEGPPX";
 53 = short,   "UEAC";
 54 = blk,     "MCL";
 55 = word,    "JRE";
 56 = chrs,    "QZCLHWF";
 57 = words,   "LORIPYC";
 58 = cmprss2, "HZX";
 59 = short,   "ITF";
 60 = cmprss2, "JIQZJ";
 61 = word,    "QQP";
 62 = one,     "ABMK";
 63 = word,    "PNLWOIR";
 64 = words,   "MXKWGSM";
 65 = opt,     "GTT";
 66 = blk,     "IQNME";
 67 = one,     "RNSDJ";
 68 = short,   "CAGO";
 69 = opt,     "MHNZOPF";
 70 = blk,     "EVTUIH";
 71 = one,     "MSTG";
 72 = chrs,    "ONGRT";
 73 = two,     "OJY";
 74 = blk,     "MTRWD";
 75 = chrs,    "YSCFTIR";
 76 = chrs,    "ZNIOARF";
 77 = cmprss,  "VKYVMKK";
 78 = opt,     "ILAC";
 79 = cmprss,  "EROOI";
 80 = one,     "KRDYS";
 81 = opt,     "OJCYKY";
 82 = opt,     "IUYQFA";
 83 = cmprss,  "VYIJN";
 84 = blk,     "HCGKAYE";
 85 = short,   "QKP";
 86 = blk,     "CYSWZKE";
 87 = chrs,    "VUDD";
 88 = words,   "VSAYB";
 89 = short,   "ZRI";
 90 = word,    "UDL";
 91 = word,    "LYFKI";
 92 = opt,     "PEG";
 93 = one,     "MLDGNDH";
 94 = one,     "JRNX";
 95 = two,     "CNN";
 96 = words,   "LQJWAVL";
 97 = chrs,    "GOPSRW";
 98 = lopt,    "FFZ";
 99 = blk,     "NNYW";
100 = opt,     "KAKXTV";
101 = cmprss2, "SHZV";
102 = short,   "LHG";
103 = two,     "EOWE";
104 = lopt,    "TSTRI";
105 = cmprss2, "KTBFQ";
106 = cmprss,  "IFADTP";
107 = blk,     "YZRCQQ";
108 = word,    "LDPGV";
109 = chrs,    "YKIBRYA";
110 = cmprss,  "FUOAJE";
111 = cmprss2, "GWOYO";
112 = opt,     "ZCKIX";
113 = cmprss,  "XWI";
114 = lopt,    "IBFRIZ";
115 = two,     "LLQUTO";
116 = opt,     "DJMCL";
117 = word,    "KHMHDU";
118 = words,   "CLYVW";
119 = lopt,    "HIQXFJL";
120 = short,   "OAPTLO";
121 = two,     "XTFCNPV";
122 = lopt,    "TNZQQGB";
123 = lopt,    "IPQXU";
124 = two,     "KCNA";
125 = one,     "YDQ";
126 = short,   "HVI";
127 = one,     "YJHOPI";
128 = word,    "QNN";
129 = one,     "WRS";
130 = word,    "UHQSMGT";
131 = word,    "QNYHLJ";
132 = blk,     "BYXVQ";
133 = one,     "HVGZ";
134 = words,   "EHO";
135 = words,   "ACXFR";
136 = chrs,    "RWIBK";
137 = opt,     "KJZAWK";
138 = blk,     "SHGZN";
139 = one,     "HURJRXA";
140 = opt,     "SDCVP";
141 = lopt,    "ACWAY";
142 = two,     "SDTA";
143 = words,   "STPOMA";
144 = words,   "DOFVT";
145 = word,    "IAPIC";
146 = cmprss,  "DWXXZ";
147 = one,     "FBUAR";
148 = cmprss,  "RQY";
149 = one,     "DRZPJHQ";
150 = one,     "UMFXIE";
151 = short,   "XWOZQO";
152 = one,     "CLQAIU";
153 = short,   "TVLD";
154 = chrs,    "XIXCHH";
155 = cmprss,  "SGC";
156 = word,    "UYZUBK";
157 = one,     "UHHW";
158 = words,   "JGQ";
159 = lopt,    "AUH";
160 = lopt,    "POWV";
161 = chrs,    "CMDB";
162 = cmprss,  "OTH";
163 = cmprss,  "PGGU";
164 = chrs,    "KMR";
165 = one,     "AXLPNH";
166 = chrs,    "ONTT";
167 = words,   "ZHX";
168 = cmprss2, "AVAGPRU";
169 = cmprss2, "LRNLO";
170 = opt,     "BIN";
171 = opt,     "DDRWGS";
172 = two,     "ZUDW";
173 = cmprss,  "OOFS";
174 = chrs,    "JHFXII";
175 = cmprss2, "VIRAN";
176 = blk,     "TDZX";
177 = blk,     "CCSOQG";
178 = cmprss,  "AORJ";
179 = blk,     "RBSJZC";
180 = two,     "PPWJUV";
181 = one,     "TMA";
182 = short,   "TSWBU";
183 = cmprss2, "DFNUHG";
184 = chrs,    "IIH";
185 = blk,     "FJRBR";
186 = short,   "DSI";
187 = blk,     "DEOXQLB";
188 = one,     "VEHBMPM";
189 = word,    "JZY";
190 = short,   "RUBV";
191 = word,    "BBMHS";
192 = words,   "DNSE";
193 = one,     "LOLDAHS";
194 = cmprss2, "TONKLO";
195 = word,    "YID";
196 = lopt,    "VYMAN";
197 = word,    "YGSMRXM";
198 = two,     "XKJO";
199 = opt,     "ZMB";
200 = lopt,    "LDADA";
201 = word,    "BOYIMG";
202 = chrs,    "YAM";
203 = opt,     "MJBAIWB";
204 = words,   "DINJLQ";
205 = chrs,    "HMA";
206 = opt,     "POZNMZC";
207 = one,     "OYUQJ";
208 = chrs,    "FSW";
209 = cmprss,  "FBXRPED";
210 = chrs,    "AGB";
211 = cmprss2, "PYYF";
212 = blk,     "FLGOD";
213 = chrs,    "XXKEF";
214 = cmprss,  "WCIZTW";
215 = cmprss2, "ZYYP";
216 = blk,     "WPMBA";
217 = lopt,    "GTKDQ";
218 = blk,     "VJL";
219 = lopt,    "TTIQUK";
220 = opt,     "KGT";
221 = two,     "JWE";
222 = word,    "WRJI";
223 = word,    "EFZPQT";
224 = words,   "MMCGZ";
225 = cmprss2, "JAXUCGQ";
226 = short,   "HGJSOG";
227 = cmprss,  "LWJDP";
228 = one,     "DARJ";
229 = short,   "NILKCPS";
230 = lopt,    "JEGU";
231 = one,     "AVPNRZR";
232 = opt,     "GLOAWN";
233 = two,     "YYVC";
234 = words,   "XNZBWVA";
235 = one,     "XILQ";
236 = one,     "YYNLM";
237 = cmprss2, "CAVAYSC";
238 = opt,     "CBJFZEF";
239 = words,   "ECS";
240 = two,     "WSMLF";
241 = two,     "NCYI";
242 = word,    "AHKKL";
243 = blk,     "QMYYGEC";
244 = two,     "XWVR";
245 = word,    "PMiller";
246 = cmprss,  "WQGY";
247 = word,    "JMFC";
248 = blk,     "CCZJHJF";
249 = two,     "AXO";
250 = opt,     "ZOKNV";
251 = one,     "BHETSS";
252 = two,     "UVBZ";
253 = word,    "GGP";
254 = blk,     "QLC";
255 = word,    "CNSP";
fubar
test $? -eq 0 || no_result

ucsdpsys_opcodes --arch=pcode-be -e opcodes.txt opcodes.bin
test $? -eq 0 || fail

ucsdpsys_opcodes --arch=pcode-be -d opcodes.bin test.out
test $? -eq 0 || fail

diff opcodes.txt test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :
