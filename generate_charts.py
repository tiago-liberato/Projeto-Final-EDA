import sys
import os
import csv
import matplotlib.pyplot as plt

os.chdir(os.path.dirname(os.path.abspath(__file__)))

def main():
    if len(sys.argv) < 2:
        print("Uso: python generate_charts.py <caminho_do_csv>")
        sys.exit(1)

    caminho = sys.argv[1]
    nome_base = os.path.basename(caminho).replace(".csv", "")

    os.makedirs("graficos", exist_ok=True)

    labels, comparacoes, tempos = [], [], []
    tem_tempo = False

    with open(caminho, newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            labels.append(row["estrutura"].strip())
            comparacoes.append(int(row["comparacoes"]))
            if "tempo" in row:
                tempos.append(float(row["tempo"]))
                tem_tempo = True

    cores = ["#4C72B0", "#DD4444", "#55A868", "#C44E52"]

    plt.figure(figsize=(8, 5))
    plt.bar(labels, comparacoes, color=cores)
    plt.title("Comparações de Chaves")
    plt.ylabel("Número de Comparações")
    plt.tight_layout()
    plt.savefig(f"graficos/{nome_base}_comparacoes.png", dpi=150)
    plt.close()
    print(f"Salvo: graficos/{nome_base}_comparacoes.png")

    if tem_tempo:
        plt.figure(figsize=(8, 5))
        plt.bar(labels, tempos, color=cores)
        plt.title("Tempo de Montagem")
        plt.ylabel("Tempo (segundos)")
        plt.tight_layout()
        plt.savefig(f"graficos/{nome_base}_tempo.png", dpi=150)
        plt.close()
        print(f"Salvo: graficos/{nome_base}_tempo.png")

if __name__ == "__main__":
    main()