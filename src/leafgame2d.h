/*
 * Description:
 * Version: 0.1
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT & Others
 */

/*
 * Copyright (c) 2021, suncloudsmoon and the LeafGame2D contributors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LEAFGAME2D_H_INCLUDED
#define LEAFGAME2D_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


/////////////////////////// START OF BRAIN.H /////////////////////////////////////////////////////////////////////////////
/*
Description: An "interface" of brain.c with a bunch of prototyped functions
Version: [Origin: 1.0], [Modified: 1.1]
Author(s): Lewis Van Winkle<LV@codeplea.com>, suncloudsmoon
Origin: https://github.com/codeplea/genann
License: Zlib
*/

/*
Disclaimer:
[Terms]
The term "software" used in this context is used to refer to the "GENANN - Minimal C Artificial Neural Network" software.
The term "we" refers to potential future developers working on this modified source version.

[Explanation]
This is an altered source version of the software and is substantially different from the original software.
Also, we did not write the original software and this is not the original software.
*/

/*
 * GENANN - Minimal C Artificial Neural Network
 *
 * Copyright (c) 2015-2018 Lewis Van Winkle
 *
 * http://CodePlea.com
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgement in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef leaf_brain_RANDOM
/* We use the following for uniform random numbers between 0 and 1.
 * If you have a better function, redefine this macro. */
#define leaf_brain_RANDOM() (((double)rand())/RAND_MAX)
#endif

struct leaf_brain;

typedef double (*leaf_brain_actfun)(const struct leaf_brain *ann, double a);

typedef struct leaf_brain {
    /* How many inputs, outputs, and hidden neurons. */
    int inputs, hidden_layers, hidden, outputs;

    /* Which activation function to use for hidden neurons. Default: gennann_act_sigmoid_cached*/
    leaf_brain_actfun activation_hidden;

    /* Which activation function to use for output. Default: gennann_act_sigmoid_cached*/
    leaf_brain_actfun activation_output;

    /* Total number of weights, and size of weights buffer. */
    int total_weights;

    /* Total number of neurons + inputs and size of output buffer. */
    int total_neurons;

    /* All weights (total_weights long). */
    double *weight;

    /* Stores input array and output of each neuron (total_neurons long). */
    double *output;

    /* Stores delta of each hidden and output neuron (total_neurons - inputs long). */
    double *delta;

} leaf_brain;

/* Creates and returns a new ann. */
leaf_brain *leaf_brain_init(int inputs, int hidden_layers, int hidden, int outputs);

/* Creates ANN from file saved with leaf_brain_write. */
leaf_brain *leaf_brain_read(FILE *in);

/* Sets weights randomly. Called by init. */
void leaf_brain_randomize(leaf_brain *ann);

/* Returns a new copy of ann. */
leaf_brain *leaf_brain_copy(leaf_brain const *ann);

/* Frees the memory used by an ann. */
void leaf_brain_free(leaf_brain *ann);

/* Runs the feedforward algorithm to calculate the ann's output. */
double const *leaf_brain_run(leaf_brain const *ann, double const *inputs);

/* Does a single backprop update. */
void leaf_brain_train(leaf_brain const *ann, double const *inputs, double const *desired_outputs, double learning_rate);

/* Saves the ann. */
void leaf_brain_write(leaf_brain const *ann, FILE *out);

void leaf_brain_init_sigmoid_lookup(const leaf_brain *ann);
double leaf_brain_act_sigmoid(const leaf_brain *ann, double a);
double leaf_brain_act_sigmoid_cached(const leaf_brain *ann, double a);
double leaf_brain_act_threshold(const leaf_brain *ann, double a);
double leaf_brain_act_linear(const leaf_brain *ann, double a);


///////////////////////////////////////////////////////////// END OF BRAIN.H ////////////////////////////////////////////////////

//////////////////////////////////////////////////////// START OF COMMANDBASICS.H ///////////////////////////////////////////////

/*
 * Description: Has functions that simply string operations, like initializing a string with certain length and setting the memory inside the string to zero all in one function and more.
 * Version: 5/29/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
*/

char* leaf_command(char *command); // Enter a command and get the data back
char* leaf_getExePath();
bool leaf_sendToastNotification();


//////////////////////////////////////////////////// END OF COMMANDBASICS.H ///////////////////////////////////////////////////

//////////////////////////////////////////// START OF FILEMANAGER.H ////////////////////////////////////////////////////////
/*
 * Description: Has functions that simply string operations, like initializing a string with certain length and setting the memory inside the string to zero all in one function and more.
 * Version: 5/29/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
*/

void leaf_saveFile(char *location, char *data);
char* leaf_retrieveFile(char *location);


//////////////////////////////////////////////////// END OF FILEMANAGER.H /////////////////////////////////////////////////

/////////////////////////////////////////////////// START OF LICENSEMANAGER.H /////////////////////////////////////////////
/*
 * Description: Has functions that simply string operations, like initializing a string with certain length and setting the memory inside the string to zero all in one function and more.
 * Version: 5/29/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
*/

#define EULA "[Insert Software Name] End-User License Agreement\n"Non-Commercial" means not primarily intended for or directed towards commercial advantage or monetary compensation. “Contributor” means each individual or legal entity that creates, contributes to the creation of, or owns Covered Software. “Contribution” means Covered Software of a particular Contributor. “Covered Software” means the preferred form for making modifications, including but not limited to software source code, documentation source, and configuration files (the "Source") and/or any form resulting from mechanical transformation or translation of a Sourceform, including but not limited to compiled object code, generated documentation, and conversions to other media types. “License” means this document. “Licensable” means having the right to grant, to the maximum extent possible, whether at the time of the initial grant or subsequently, any and all of the rights conveyed by this License. “Patent Claims” of a Contributor means any patent claim(s), including without limitation, method, process, and apparatus claims, in any patent Licensable by such Contributor that would be infringed, but for the grant of the License, by the making, using, selling, offering for sale, having made, import, or transfer of either its Contributions or its Contributor Version. “You” (or “Your”) means an individual or a legal entity exercising rights under this License. For legal entities, “You” includes any entity that controls, is controlled by, or is under common control with You. For purposes of this definition, “control” means (a) the power, direct or indirect, to cause the direction or management of such entity, whether by contract or otherwise, or (b) ownership of more than fifty percent (50%) of the outstanding shares or beneficial ownership of such entity.\nThis License is not intended to limit any rights You have under applicable copyright doctrines of fair use, fair dealing, or other equivalents. No Covered Software shall be deemed part of an effective technological measure under any applicable law fulfilling obligations under article 11 of the WIPO copyright treaty adopted on 20 December 1996, or similar laws prohibiting or restricting circumvention of such measures. When Contributors convey a covered work, the Contributors waive any legal power to forbid circumvention of technological measures to the extent such circumvention is effected by exercising rights under this License with respect to the covered work, and the Contributors disclaim any intention to limit operation or modification of the work as a means of enforcing, against the work's users, Contributors or third parties' legal rights to forbid circumvention of technological measures.\nEach Contributor hereby grants You a world-wide, royalty-free, sublicensable, perpetual, non-commercial, non-exclusive license under intellectual property rights (other than patent or trademark) Licensable by such Contributor to use, reproduce, make available, modify, display, perform, distribute, and otherwise exploit its Contributions; and under Patent Claims of such Contributor to make, use, sell, offer for sale, have made, import, and otherwise transfer either its Contributions or its Contributor Version (be collectively known as the "Freedom Grant").\nAfter 8 years from the last copyrighted year stated in the Copyright Notice, each Contributor hereby grants You permission to relicense the Covered Software to a new software license that is stated below (the "Dirt License"):\n"Copyright © [Some Year] - [Some Year], [Project Founder] and the [Project Name] Contributors. All Rights Reserved.\n “License” means the copyright notice(s), definitions (marked by quotes), permissions notice, and liability disclaimer. “Licensable” means having the right to grant, to the maximum extent possible, whether at the time of the initial grant or subsequently, any and all of the rights conveyed by this License. “Contributor” means each individual or legal entity that creates, contributes to the creation of, or owns Covered Software. "You” (or “Your”) means an individual or a legal entity exercising rights under this License. For legal entities, “You” includes any entity that controls, is controlled by, or is under common control with You. For purposes of this definition, “control” means (a) the power, direct or indirect, to cause the direction or management of such entity, whether by contract or otherwise, or (b) ownership of more than fifty percent (50%) of the outstanding shares or beneficial ownership of such entity. “Contribution” means Covered Software of a particular Contributor. “Covered Software” means the preferred form for making modifications, including but not limited to software source code, documentation source, and configuration files (the "Source") and/or any form resulting from mechanical transformation or translation of a Sourceform, including but not limited to compiled object code, generated documentation, and conversions to other media types. “Patent Claims” of a Contributor means any patent claim(s), including without limitation, method, process, and apparatus claims, in any patent Licensable by such Contributor that would be infringed, but for the grant of the License, by the making, using, selling, offering for sale, having made, import, or transfer of either its Contributions or its Contributor Version.\nEach Contributor hereby grants You a world-wide, royalty-free, sublicensable, perpetual, irrevocable, non-exclusive license under intellectual property rights (other than patent or trademark) Licensable by such Contributor to use, reproduce, make available, modify, display, perform, distribute, and otherwise exploit its Contributions; and under Patent Claims of such Contributor to make, use, sell, offer for sale, have made, import, and otherwise transfer either its Contributions or its Contributor Version, subject to the following conditions:\nThis License shall be included in all copies or substantial portions of the Covered Software.\nANY LITIGATION RELATING TO THIS LICENSE MAY BE BROUGHT ONLY IN THE COURTS OF A JURISDICTION WHERE THE DEFENDANT MAINTAINS ITS PRINCIPAL PLACE OF BUSINESS AND SUCH LITIGATION SHALL BE GOVERNED BY LAWS OF THAT JURISDICTION, WITHOUT REFERENCE TO ITS CONFLICT-OF-LAW PROVISIONS. NOTHING IN THIS SECTION SHALL PREVENT A PARTY’S ABILITY TO BRING CROSS-CLAIMS OR COUNTER-CLAIMS. THIS LICENSE REPRESENTS THE COMPLETE AGREEMENT CONCERNING THE SUBJECT MATTER HEREOF. IF ANY PROVISION OF THIS LICENSE IS HELD TO BE UNENFORCEABLE, SUCH PROVISION SHALL BE REFORMED ONLY TO THE EXTENT NECESSARY TO MAKE IT ENFORCEABLE. ANY LAW OR REGULATION WHICH PROVIDES THAT THE LANGUAGE OF A CONTRACT SHALL BE CONSTRUED AGAINST THE DRAFTER SHALL NOT BE USED TO CONSTRUE THIS LICENSE AGAINST A CONTRIBUTOR. COVERED SOFTWARE IS PROVIDED UNDER THIS LICENSE ON AN “AS IS” BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED SOFTWARE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED SOFTWARE IS WITH YOU. SHOULD ANY COVERED SOFTWARE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT ANY CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR, OR CORRECTION. UNDER NO CIRCUMSTANCES AND UNDER NO LEGAL THEORY, WHETHER TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE, SHALL ANY CONTRIBUTOR, OR ANYONE WHO DISTRIBUTES COVERED SOFTWARE AS PERMITTED ABOVE, BE LIABLE TO YOU FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OF ANY CHARACTER INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOST PROFITS, LOSS OF GOODWILL, WORK STOPPAGE, COMPUTER FAILURE OR MALFUNCTION, OR ANY AND ALL OTHER COMMERCIAL DAMAGES OR LOSSES, EVEN IF SUCH PARTY SHALL HAVE BEEN INFORMED OF THE POSSIBILITY OF SUCH DAMAGES. THIS LIMITATION OF LIABILITY SHALL NOT APPLY TO LIABILITY FOR DEATH OR PERSONAL INJURY RESULTING FROM SUCH PARTY’S NEGLIGENCE TO THE EXTENT APPLICABLE LAW PROHIBITS SUCH LIMITATION. SOME JURISDICTIONS DO NOT ALLOW THE EXCLUSION OR LIMITATION OF INCIDENTAL OR CONSEQUENTIAL DAMAGES, SO THIS EXCLUSION AND LIMITATION MAY NOT APPLY TO YOU."\nThe rights granted in the Freedom Grant are irrevocable, subject to the following conditions:\nThe rights granted under this License will terminate automatically if You fail to comply with any of its terms. However, if You become compliant, then the rights granted under this License from a particular Contributor are reinstated (a) provisionally, unless and until such Contributor explicitly and finally terminates Your grants, and (b) on an ongoing basis, if such Contributor fails to notify You of the non-compliance by some reasonable means prior to 60 days after You have come back into compliance. Moreover, Your grants from a particular Contributor are reinstated on an ongoing basis if such Contributor notifies You of the non-compliance by some reasonable means, this is the first time You have received notice of non-compliance with this License from such Contributor, and You become compliant prior to 30 days after Your receipt of the notice. If You initiate litigation against any entity by asserting a patent infringement claim (excluding declaratory judgment actions, counter-claims, and cross-claims) alleging that a Contributor Version directly or indirectly infringes any patent, then the rights granted to You by any and all Contributors for the Covered Software under the Freedom Grant of this License shall terminate. In the event of termination, all end user license agreements (excluding distributors and resellers) which have been validly granted by You or Your distributors under this License prior to termination shall survive termination.\nANY LITIGATION RELATING TO THIS LICENSE MAY BE BROUGHT ONLY IN THE COURTS OF A JURISDICTION WHERE THE DEFENDANT MAINTAINS ITS PRINCIPAL PLACE OF BUSINESS AND SUCH LITIGATION SHALL BE GOVERNED BY LAWS OF THAT JURISDICTION, WITHOUT REFERENCE TO ITS CONFLICT-OF-LAW PROVISIONS. NOTHING IN THIS SECTION SHALL PREVENT A PARTY’S ABILITY TO BRING CROSS-CLAIMS OR COUNTER-CLAIMS. THIS LICENSE REPRESENTS THE COMPLETE AGREEMENT CONCERNING THE SUBJECT MATTER HEREOF. IF ANY PROVISION OF THIS LICENSE IS HELD TO BE UNENFORCEABLE, SUCH PROVISION SHALL BE REFORMED ONLY TO THE EXTENT NECESSARY TO MAKE IT ENFORCEABLE. ANY LAW OR REGULATION WHICH PROVIDES THAT THE LANGUAGE OF A CONTRACT SHALL BE CONSTRUED AGAINST THE DRAFTER SHALL NOT BE USED TO CONSTRUE THIS LICENSE AGAINST A CONTRIBUTOR. COVERED SOFTWARE IS PROVIDED UNDER THIS LICENSE ON AN “AS IS” BASIS, WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, WITHOUT LIMITATION, WARRANTIES THAT THE COVERED SOFTWARE IS FREE OF DEFECTS, MERCHANTABLE, FIT FOR A PARTICULAR PURPOSE OR NON-INFRINGING. THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE COVERED SOFTWARE IS WITH YOU. SHOULD ANY COVERED SOFTWARE PROVE DEFECTIVE IN ANY RESPECT, YOU (NOT ANY CONTRIBUTOR) ASSUME THE COST OF ANY NECESSARY SERVICING, REPAIR, OR CORRECTION. UNDER NO CIRCUMSTANCES AND UNDER NO LEGAL THEORY, WHETHER TORT (INCLUDING NEGLIGENCE), CONTRACT, OR OTHERWISE, SHALL ANY CONTRIBUTOR, OR ANYONE WHO DISTRIBUTES COVERED SOFTWARE AS PERMITTED ABOVE, BE LIABLE TO YOU FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OF ANY CHARACTER INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOST PROFITS, LOSS OF GOODWILL, WORK STOPPAGE, COMPUTER FAILURE OR MALFUNCTION, OR ANY AND ALL OTHER COMMERCIAL DAMAGES OR LOSSES, EVEN IF SUCH PARTY SHALL HAVE BEEN INFORMED OF THE POSSIBILITY OF SUCH DAMAGES. THIS LIMITATION OF LIABILITY SHALL NOT APPLY TO LIABILITY FOR DEATH OR PERSONAL INJURY RESULTING FROM SUCH PARTY’S NEGLIGENCE TO THE EXTENT APPLICABLE LAW PROHIBITS SUCH LIMITATION. SOME JURISDICTIONS DO NOT ALLOW THE EXCLUSION OR LIMITATION OF INCIDENTAL OR CONSEQUENTIAL DAMAGES, SO THIS EXCLUSION AND LIMITATION MAY NOT APPLY TO YOU.\nEnter 'y' to indicate that you have read and agree to the [Insert Software Name] End-User License Agreement and enter 'n' to indicate otherwise."

////////////////////////////////////////////////// END OF LICENSEMANAGER.H //////////////////////////////////////////////

////////////////////////////////////////////////////// START OF LOGGER.H ////////////////////////////////////////////////

/*
 * Description: Has functions that simply string operations, like initializing a string with certain length and setting the memory inside the string to zero all in one function and more.
 * Version: 5/29/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
*/

/// Logs data to file on function call ///

void leaf_setLogger(char *location);
void leaf_log(char *message);
void leaf_free();

/////////////////////////////////////////////// END OF LOGGER.H ////////////////////////////////////////////////////

///////////////////////////////////////////// START OF NOOBENCRYPTION.H /////////////////////////////////////////////
/*
 * Description: Has functions that simply string operations, like initializing a string with certain length and setting the memory inside the string to zero all in one function and more.
 * Version: 5/29/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
*/

// A very secure encryption and fast algorithm because it is not known to the world and it uses shift based encryption
char* leaf_encrypt(double key, char *data);
char* leaf_decrypt(double key, char *data);

void leaf_setCryptoSeed(long double seed);
int leaf_cryptorand();

/////////////////////////////////////////////// END OF NOOBENCRYPTION.H /////////////////////////////////////////////////////

////////////////////////////////////////////// START OF PERLIN.H ///////////////////////////////////////////////////////////
/*
Description: An "interface" for perlin.c, which contains a few prototype functions
Version: [Origin: Jun 8, 2016], [Modified: April 19, 2021]
Author(s): Stefan Gustavson<stefan.gustavson@liu.se>, suncloudsmoon
Origin: https://github.com/stegu/perlin-noise
License: Public Domain
*/

//////////////////////// Original Author Stuff [Start] /////////////////////////////////////////////

// noise1234
//
// Author: Stefan Gustavson, 2003-2005
// Contact: stefan.gustavson@liu.se
//
// This code was GPL licensed until February 2011.
// As the original author of this code, I hereby
// release it into the public domain.
// Please feel free to use it for whatever you want.
// Credit is appreciated where appropriate, and I also
// appreciate being told where this code finds any use,
// but you may do as you like.

/*
 * This implementation is "Improved Noise" as presented by
 * Ken Perlin at Siggraph 2002. The 3D function is a direct port
 * of his Java reference code which was once publicly available
 * on www.noisemachine.com (although I cleaned it up, made it
 * faster and made the code more readable), but the 1D, 2D and
 * 4D functions were implemented from scratch by me.
 *
 * This is a backport to C of my improved noise class in C++
 * which was included in the Aqsis renderer project.
 * It is highly reusable without source code modifications.
 *
 */

//////////////////////// Original Author Stuff [End] /////////////////////////////////////////////

// Generate 1D and 2D perlin noise with the ability t
float leaf_generate1DPerlinNoise(float x);
float leaf_generate2DPerlinNoise(float x, float y);
void leaf_setSeed(int seed);

///////////////////////////////////////////////////////// END OF PERLIN.H ///////////////////////////////////////////////

//////////////////////////////////////////////////////// START OF SIMPLEMALLOC.H /////////////////////////////////////////
/*
 * Description: Has functions that simply string operations, like initializing a string with certain length and setting the memory inside the string to zero all in one function and more.
 * Version: 5/29/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
*/

char* leaf_charmalloc(int length);
int* leaf_intmalloc(int length);
float* leaf_floatmalloc(int length);

////////////////////////////////////////////////////// END OF SIMPLEMALLOC.H ////////////////////////////////////////////////

////////////////////////////////////////////////////// START OF STRINGBASICS.H /////////////////////////////////////////////
/*
 * Description: Has functions that simply string operations, like initializing a string with certain length and setting the memory inside the string to zero all in one function and more.
 * Version: 5/29/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
*/

char* leaf_initString(int length);
void leaf_memToZero(void* item);
char* leaf_generateWord(int num);
void leaf_addStrings(char *dest, char *one, char *two, char *three);


///////////////////////////////////////// END OF STRINGBASICS.H //////////////////////////////////////////////////////////

///////////////////////////////////////// START OF URLCREATOR.H //////////////////////////////////////////////////////////
/*
 * Description: A simple system in which you can create a .url file by setting the attributes of a shortcut using the struct Shortcut descripted in urlcreator.h
 * Version: 5/29/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
*/

typedef struct {
    char *url;
    char *iconDir;
    char *dest;

    int rating;
    char *description;
    char *notes;
} leaf_shortcut;

void leaf_createURLShortcut(leaf_shortcut *sh);

////////////////////////////////////////////////// END OF URLCREATOR.H ///////////////////////////////////////////////////

///////////////////////////////////////////////// START OF WJCRYPTOLIB_SHA256.H //////////////////////////////////////////
/*
 * Description: A simple system in which you can create a .url file by setting the attributes of a shortcut using the struct Shortcut descripted in urlcreator.h
 * Version: 5/29/2021
 * Author(s): suncloudsmoon
 * Origin: N/A
 * License: MIT
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  WjCryptLib_Sha256
//
//  Implementation of SHA256 hash function.
//  Original author: Tom St Denis, tomstdenis@gmail.com, http://libtom.org
//  Modified by WaterJuice retaining Public Domain license.
//
//  This is free and unencumbered software released into the public domain - June 2013 waterjuice.org
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  IMPORTS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <stdint.h>
//#include <stdio.h>

typedef struct
{
    uint64_t    length;
    uint32_t    state[8];
    uint32_t    curlen;
    uint8_t     buf[64];
} Sha256Context;

#define SHA256_HASH_SIZE           ( 256 / 8 )

typedef struct
{
    uint8_t      bytes [SHA256_HASH_SIZE];
} SHA256_HASH;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  PUBLIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Initialise
//
//  Initialises a SHA256 Context. Use this to initialise/reset a context.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Sha256Initialise
    (
        Sha256Context*      Context         // [out]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Update
//
//  Adds data to the SHA256 context. This will process the data and update the internal state of the context. Keep on
//  calling this function until all the data has been added. Then call Sha256Finalise to calculate the hash.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Sha256Update
    (
        Sha256Context*      Context,        // [in out]
        void const*         Buffer,         // [in]
        uint32_t            BufferSize      // [in]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Finalise
//
//  Performs the final calculation of the hash and returns the digest (32 byte buffer containing 256bit hash). After
//  calling this, Sha256Initialised must be used to reuse the context.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Sha256Finalise
    (
        Sha256Context*      Context,        // [in out]
        SHA256_HASH*        Digest          // [out]
    );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Sha256Calculate
//
//  Combines Sha256Initialise, Sha256Update, and Sha256Finalise into one function. Calculates the SHA256 hash of the
//  buffer.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
    Sha256Calculate
    (
        void  const*        Buffer,         // [in]
        uint32_t            BufferSize,     // [in]
        SHA256_HASH*        Digest          // [in]
    );

///////////////////////////////////////////////// END OF WJCRYPTOLIB_SHA256.H //////////////////////////////////////////

#endif // LEAFGAME2D_H_INCLUDED
