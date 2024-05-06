# Ray-tracing-project
*O que é?* É uma técnica da computação gráfica que simula os raios de luz para criação de cenários de iluminação específicos, como sombras, reflexos em superfícies ou até a iluminação geral de um jogo, tornando ele mais realista e mais bonito.

*Se um objeto emite milhões de raios de luz, como é possível traçar todos os raios para formar uma imgaem usando ray-traing?* Na verdade, o que o ray-traicing faz é pegar somente os raios que partem, por exemplo, do seu personagem. Ele, basicamente, faz o caminho inverso da luz. Naturalmente, a luz emite os raios, que batem em um objeto e são refletidos para seu olho (câmera). Aqui, ***simulamos o raio de luz que parte da câmera, vai até o objeto e chega, por fim, na fonte de luz***. Assim, define-se quais são os raios necessários para se usar naquele momento para ***cada pixel***.

## Visão geral
O projeto será dividido em ***seis entregas,*** que consistem em: ***implementar um ray-tracer básico, interseção com malhas de triângulos, transformações afins, modelo de iluminação de phong, ray-tracer recursivo e por fim a(s) feature(s) extra(s)***; Cada entrega será corrigida individualmente e a avaliação também será individual. Na pagina de cada entrega é possível ver mais detalhes a respeito.

Tenha em mente que toda vez que nos referimos a ***pontos ou vetores*** nessa especificação, nos referimos a ***duas estruturas de dados compostas por 3 coordenadas (x, y, z), podendo ser floats ou doubles (recomendamos double para evitar erros de precisão e arredondamento de casas decimais).*** Tente usar apenas um dos dois tipos por toda a sua aplicação (floats ou double). Além disso, a ***cor RGB*** é formada por 3 parâmetros, um para o vermelho (R), um para o verde (G) e um para o azul (B), ***você pode armazenar essa cor em um vetor com 3 coordenadas***; porém, 0 ⩽ R, G, B ⩽ 255, podendo ser *floats* ou *doubles* (de novo só pra reforçar). ***Criem um repositório no github para gerenciar o projeto e facilitar a nossa correção.***

***OBS:***  é extremamente recomendado que vocês usem C++. Todo material de suporte é baseado em C++ e a linguagem oferece *operator overloading*, que se mostrará muito útil para trabalhar com pontos, vetores e transformações afins. (Apesar disso, não é obrigatório, o verdadeiro ray-tracing são os amigos que fazemos no caminho).

## Links importantes
- Especificação (Notion): https://short-crustacean-7e9.notion.site/71c2663a94304d629d33a8f289a7017e?v=96ba08a6ebe647e3aa5339ea8e019f3b