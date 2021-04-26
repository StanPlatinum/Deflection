#include "bpnn_fit.h"
#include "bpnn_config.h"

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <fcntl.h>

#define T bpnn_t

#define D IN_N
#define Q HIDDEN_N
#define L OUT_N


#include "CFICheck.h"
#include "enclave.h"

struct T
{
    double *v[D];
    double *w[Q];
    double *r;
    double *o;
    double *b;
};

static bool get_parameter(T bpnn);

T bpnn_fit_new(void)
{
    T bpnn;

    bpnn = malloc(sizeof(*bpnn));
    if (bpnn == NULL)
        goto cleanup;

    for (size_t i = 0; i < D; i++)
    {
        bpnn->v[i] = malloc(sizeof(double) * Q);
        if (bpnn->v[i] == NULL)
            goto cleanup;
    }

    for (size_t h = 0; h < Q; h++)
    {
        bpnn->w[h] = malloc(sizeof(double) * L);
        if (bpnn->w[h] == NULL)
            goto cleanup;
    }

    bpnn->r = malloc(sizeof(double) * Q);
    if (bpnn->r == NULL)
        goto cleanup;
    bpnn->o = malloc(sizeof(double) * L);
    if (bpnn->o == NULL)
        goto cleanup;

    bpnn->b = malloc(sizeof(double) * Q);
    if (bpnn->b == NULL)
        goto cleanup;

    if (!get_parameter(bpnn))
    {
        puts("[BPNN] GET PARAMETER FAILED!\n");
        goto cleanup;
    }

    return bpnn;

cleanup:
    if (bpnn)
    {
        for (size_t i = 0; i < D; i++)
        {
            if (bpnn->v[i] != NULL)
                free(bpnn->v[i]);
        }

        for (size_t h = 0; h < Q; h++)
        {
            if (bpnn->w[h] != NULL)
                free(bpnn->w[h]);
        }

        if (bpnn->r != NULL)
            free(bpnn->r);
        if (bpnn->o != NULL)
            free(bpnn->o);
        if (bpnn->b != NULL)
            free(bpnn->b);
        free(bpnn);
    }

    return NULL;
}

void bpnn_fit(T bpnn, double *in, double *out)
{
    assert(bpnn && in && out);

    /* Compute b[h] */
    for (size_t h = 0; h < Q; h++)
    {
        double alpha_h = 0;
        for (size_t i = 0; i < D; i++)
            alpha_h += bpnn->v[i][h] * in[i];
        bpnn->b[h] = ACTIVATION_FUNC(alpha_h - bpnn->r[h]);
    }

    /* Compute out[j] */
    for (size_t j = 0; j < L; j++)
    {
        double beta_j = 0;
        for (size_t h = 0; h < Q; h++)
            beta_j += bpnn->w[h][j] * bpnn->b[h];
        out[j] = ACTIVATION_FUNC(beta_j - bpnn->o[j]);
    }
}

void bpnn_fit_free(T *bpnn)
{
    assert(bpnn && *bpnn);

    for (size_t i = 0; i < D; i++)
        free((*bpnn)->v[i]);

    for (size_t h = 0; h < Q; h++)
        free((*bpnn)->w[h]);

    free((*bpnn)->r);
    free((*bpnn)->o);
    free((*bpnn)->b);

    *bpnn = NULL;
}

//Weijie:
// n should be larger than 1
// buf size should be less than n
int readline(int fd, char *buf, size_t n)
{
	for(int i = 0; i < n - 1; i++){
		char a;
		if (read(fd, &a, 1) != NULL) {
			if (a != '\n') {
				buf[i] = a;
			} else {
				buf[i] = '\n';
				buf[i+1] = '\0';
				return (i+1);
			}
		}
		else {
			buf[i+1] = '\0';
			return (i+1);
		}
	}
	buf[n] = '\0';
	return n;
}

static bool get_parameter(T bpnn)
{
#define BUFFER_SIZE 128
    /*
	   FILE *in = NULL;
	   in = fopen(SAVE_PARAM_PATH, "r");
	   if (in == NULL) {
	        //fprintf(stderr, "[BPNN] OPEN FILE %s FAILED.\n", SAVE_PARAM_PATH);
	        return false;
	    }
	 */
    int in_fd;
    in_fd = open(SAVE_PARAM_PATH, O_RDONLY, S_IRUSR);
    if (in_fd == -1)
    {
        puts("File cannot be opened!\n");
        return false;
    }

    char buffer[BUFFER_SIZE];

    if (read(in_fd, buffer, BUFFER_SIZE) != NULL)
    {
        puts("test\n");
        puts(buffer);
    }
    puts("dbg0.5\n");
    //for (size_t i = 0; (i < 5) && (fgets(buffer, BUFFER_SIZE, in) != NULL); i++) {
    //for (size_t i = 0; (i < 5) && (read(in_fd, buffer, BUFFER_SIZE) != NULL); i++)
    for (size_t i = 0; (i < 5) && (readline(in_fd, buffer, BUFFER_SIZE) > 0); i++)
    {

        puts("Buffer:\n");
        puts(buffer);

        if (buffer[0] == '#')
            continue;
        else if (buffer[0] == 'D' && buffer[1] == '=')
        {
            if (strtol(buffer + 2, NULL, 0) != D)
            {
                goto cleanup;
            }
        }
        else if (buffer[0] == 'Q' && buffer[1] == '=')
        {
            if (strtol(buffer + 2, NULL, 0) != Q)
            {
                goto cleanup;
            }
        }
        else if (buffer[0] == 'L' && buffer[1] == '=')
        {
            if (strtol(buffer + 2, NULL, 0) != L)
            {
                goto cleanup;
            }
        }
    }

    //puts("dbg1\n");

    for (size_t i = 0; i < D; i++)
        for (size_t h = 0; h < Q; h++)
        {
            //if (fgets(buffer, BUFFER_SIZE, in) != NULL) {
            //if (read(in_fd, buffer, BUFFER_SIZE) != NULL)
            if (readline(in_fd, buffer, BUFFER_SIZE) > 0)
            {
                bpnn->v[i][h] = strtod(buffer, NULL);
            }
            else
            {
                goto cleanup;
            }
        }

    //puts("dbg2\n");

    for (size_t h = 0; h < Q; h++)
        for (size_t j = 0; j < L; j++)
        {
            //if (fgets(buffer, BUFFER_SIZE, in) != NULL) {
            //if (read(in_fd, buffer, BUFFER_SIZE) != NULL)
            if (readline(in_fd, buffer, BUFFER_SIZE) > 0)
            {
                bpnn->w[h][j] = strtod(buffer, NULL);
            }
            else
            {
                goto cleanup;
            }
        }
    for (size_t h = 0; h < Q; h++)
    {
        //if (fgets(buffer, BUFFER_SIZE, in) != NULL) {
        //if (read(in_fd, buffer, BUFFER_SIZE) != NULL)
        if (readline(in_fd, buffer, BUFFER_SIZE) > 0)
        {
            bpnn->r[h] = strtod(buffer, NULL);
        }
        else
        {
            goto cleanup;
        }
    }
    for (size_t j = 0; j < L; j++)
    {
        //if (fgets(buffer, BUFFER_SIZE, in) != NULL) {
        //if (read(in_fd, buffer, BUFFER_SIZE) != NULL)
        if (readline(in_fd, buffer, BUFFER_SIZE) > 0)
        {
            bpnn->o[j] = strtod(buffer, NULL);
        }
        else
        {
            goto cleanup;
        }
    }

	//puts("dbg2.5\n");
    //fclose(in);
    close(in_fd);
    return true;

cleanup:
    //fprintf(stderr, "[BPNN] BPNN PARAM FILE NOT FIT!\n");
    puts("clean up: BPNN PARAM FILE NOT FIT!\n");
    //fclose(in);
    close(in_fd);
    return false;
}

#undef T

static bool test_set_get(double *in, double *out);

//static FILE *in_file = NULL;
//static FILE *out_file = NULL;

static int in_file_fd = 0;
static int out_file_fd = 0;

void enclave_main(void)
{
    unsigned long long magic = 0;
    puts("enclave begins...\n");

    bpnn_t bpnn = bpnn_fit_new();
    if (bpnn == NULL)
    {
        //fprintf(stderr, "bpnn new failed!\n");
        puts("bpnn new failed!\n");
        //return;
    }
    /*
		in_file = fopen(IN_PATH, "r");
		if (in_file == NULL) {
		//fprintf(stderr, "open file %s failed.\n", IN_PATH);
		puts("open file failed!\n");
		return;
		}
	 */

    in_file_fd = open(TEST_IN_PATH, O_RDONLY, S_IRUSR);
	//puts("dbg3\n");
    
	if (in_file_fd == -1)
    {
        puts("open file failed!\n");
        //return;
    }
	
    /*
		out_file = fopen(OUT_PATH, "r");
		if (out_file == NULL) {
		//fprintf(stderr, "open file %s failed.\n", OUT_PATH);
		puts("open file failed!\n");
		return;
		}
	 */
    out_file_fd = open(TEST_OUT_PATH, O_RDWR, S_IRUSR);
    if (out_file_fd == -1)
    {
        puts("open file failed!\n");
        //return;
    }

    puts("start to run...\n");

    double in[IN_N], out[OUT_N], outy[OUT_N];
    while (test_set_get(in, out))
    {
        bpnn_fit(bpnn, in, outy);
        for (size_t i = 0; i < IN_N; i++)
            //printf("%lf ", in[i]);
            puts("processing IN...\n");
        for (size_t j = 0; j < OUT_N; j++)
            //printf("%lf[%lf] ", out[j], outy[j]);
            puts("processing OUT...\n");
        //printf("\n");
    }

    bpnn_fit_free(&bpnn);

    //fclose(in_file);
    //fclose(out_file);
    close(in_file_fd);
    close(out_file_fd);

    puts("success!\n");
    //return;
    enclave_exit();
}

static bool test_set_get(double *in, double *out)
{
    assert(in && out);
#define BUFFER_SIZE 128
    static char buffer[BUFFER_SIZE];

    //if (in_file && out_file) {
    if (in_file_fd && out_file_fd)
    {
        //if (fgets(buffer, BUFFER_SIZE, in_file) != NULL) {
        //if (read(in_file_fd, buffer, BUFFER_SIZE) != NULL)
        if (readline(in_file_fd, buffer, BUFFER_SIZE) > 0)
        {
            char *token = strtok(buffer, ",");
            for (size_t i = 0; i < IN_N; i++)
            {
                if (token == NULL)
                {
                    //fprintf(stderr, "the format of input is not correct!\n");
                    return false;
                }
                in[i] = strtod(token, NULL);
                token = strtok(NULL, ",");
            }
        }
        else
        {
            return false;
        }

        //if (fgets(buffer, BUFFER_SIZE, out_file) != NULL) {
        //if (read(out_file_fd, buffer, BUFFER_SIZE) != NULL)
        if (readline(out_file_fd, buffer, BUFFER_SIZE) > 0)
        {
            out[0] = strtod(buffer, NULL);
        }
        else
        {
            return false;
        }
        return true;
    }
    return false;
}
