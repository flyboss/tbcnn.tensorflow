

public class SelectionSort{

    public static void main(String[] args){
        int[] meuArray = {3, 8, 15, 10, 1, 2, 3};
        exibirValores(meuArray);
        ordenarSelectionSort(meuArray);
        exibirValores(meuArray);
    }

    private static void ordenarSelectionSort(int meuArray[]){

        for(int i = 0; i < meuArray.length - 1; ++i){
            int indiceMenorNumero = i;

            for(int j = i + 1; j < meuArray.length; j++){
                if(meuArray[j] < meuArray[indiceMenorNumero]){
                    indiceMenorNumero = j;
                }
            }

            int temporario = meuArray[indiceMenorNumero];
            meuArray[indiceMenorNumero] = meuArray[i];
            meuArray[i] = temporario;
        }
    }

    private static void exibirValores(int meuArray[]){
        for(int i = 0; i < meuArray.length; ++i){
            System.out.print(meuArray[i] + " ");
        }
        System.out.println();
    }
    
}
