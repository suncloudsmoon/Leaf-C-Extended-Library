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


#ifndef BRAIN_H
#define BRAIN_H

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


#ifdef __cplusplus
}
#endif

#endif /*BRAIN_H*/
