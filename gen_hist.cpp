#include <stdio.h>
#include <stdlib.h>

#define OUTPUT_FILE "hist.py"

int gen_hist(int ht_size, int *arr)
{
    FILE *file = fopen("plot_hist.py", "w"); 
    if (file == NULL) return -1;

    fprintf(file, "import matplotlib.pyplot as plt\n\n");

    fprintf(file, "data = [");
    for (int i = 0; i < ht_size; i++)
    {
        fprintf(file, "%d%s", arr[i], (i < ht_size - 1) ? ", " : "");
    }
    fprintf(file, "]\n\n");

    fprintf(file, "plt.figure(figsize=(12, 6))\n");
    fprintf(file, "plt.bar(range(%d), data, color='skyblue', edgecolor='black', width=1.0)\n\n", ht_size);

    fprintf(file, "plt.xlabel('Номер ячейки')\n");
    fprintf(file, "plt.ylabel('Load-фактор')\n");
    fprintf(file, "plt.title('Load-фактор по ячейкам хеш-таблицы')\n");
    fprintf(file, "plt.tight_layout()\n");
    fprintf(file, "plt.show()\n");

    fclose(file);
    return 0;
}