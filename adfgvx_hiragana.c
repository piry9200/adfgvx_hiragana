// 'A'はA\0を組み合わせたもの. 'A'はA. c言語は'と'を区別する.
// scanfは空白を取得できない

#include <stdio.h>
#include <wchar.h>
#include <locale.h> 

int isEnd(wchar_t string[]); //指定したワイド文字列配列の中で一番後ろにある文字のインデックス番号を返す.一文字も入ってない場合-1を返す.

int main(int argc, const char * argv[]){
    setlocale(LC_CTYPE, ""); //ワイド文字列を出力するために必要
    wchar_t key[9] = {L'き',L'さ',L'ま',L'み',L'て',L'い',L'る',L'な',L'!'};
    wchar_t table[9][9] ={
            {L'あ',L'い',L'う',L'え',L'お',L'か',L'き',L'く',L'け'},
            {L'こ',L'さ',L'し',L'す',L'せ',L'そ',L'た',L'ち',L'つ'},
            {L'て',L'と',L'な',L'に',L'ぬ',L'ね',L'の',L'は',L'ひ'},
            {L'ふ',L'へ',L'ほ',L'ま',L'み',L'む',L'め',L'も',L'や'},
            {L'ゆ',L'よ',L'ら',L'り',L'る',L'れ',L'ろ',L'わ',L'を'},
            {L'ん',L'が',L'ぎ',L'ぐ',L'げ',L'ご',L'ざ',L'じ',L'ず'},
            {L'ぜ',L'ぞ',L'だ',L'ぢ',L'づ',L'で',L'ど',L'ば',L'び'},
            {L'ぶ',L'べ',L'ぼ',L'ぱ',L'ぴ',L'ぷ',L'ぺ',L'ぽ',L'0'},
            {L'1',L'2',L'3',L'4',L'5',L'6',L'7',L'8',L'9'}
        };

    wchar_t scan[501] = {L'\0'};
    int flag_1 = 0;
    int indexToInput = 0;

    printf("使用可能文字: ひらがな, 半角数字\n");
    printf("改行 : Enterキー\n");
    printf("空白 : _ または ＿\n");
    printf("文を入力してください。(q(半角or全角)で終了)\n");

    while(1){
        indexToInput = isEnd(scan); //scanfする前の最後尾にあるインデックス番号を代入
        scanf("%ls",&scan[indexToInput + 1]);
        indexToInput = isEnd(scan); //scanf後の最後尾にあるインデックス番号を代入
        if(scan[indexToInput] == 'q'){
            break;
        }
        scan[indexToInput + 1] = '\n'; //一度Enterキーが押されたら改行する.
    }
    
    for(int i = 0; i < sizeof(scan); i++){ //scanの文字をひとつずつチェック

        if(scan[i] == '\0'){ //scanから取り出した値が'\0'だったら全ての文字を見終えたのでプログラムを終了する
            printf("\n");
            return 0;
        } else if(scan[i] == '_' || scan[i] == L'＿'){
            printf(" ");
            continue;
        } else if(scan[i] == '\n'){
            printf("\n");
            continue;
        }

        for(int r = 0; r < 9; r++){  //scanから取り出した文字が表の何行何列にあるかを確かめる r=行 c=列
            for(int c = 0; c < 9; c++){
                if(scan[i] == table[r][c]){
                    wprintf(L"%lc%lc", key[r], key[c]);
                    flag_1 = 1; //表から文字が見つかったらそれを出力してflagを立てる
                    break; //列のインクリメントを中断
                }
            }
            if(flag_1 == 1){ //値が見つかったら,行のインクリメントを中断
                flag_1 = 0;
                break;
            }
        }
    }
        
    return 0;
}


int isEnd(wchar_t string[]){
    int count;
    for(count = 0; string[count] != '\0'; count++){

    }
    return count - 1; //配列の最後尾にある文字のインデックス番号を返す.
    }