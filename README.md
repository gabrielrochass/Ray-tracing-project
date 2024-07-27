# Ray-tracing-project
*O que é?* É uma técnica da computação gráfica que simula os raios de luz para criação de cenários de iluminação específicos, como sombras, reflexos em superfícies ou até a iluminação geral de um jogo, tornando ele mais realista e mais bonito.

*Se um objeto emite milhões de raios de luz, como é possível traçar todos os raios para formar uma imgaem usando ray-traing?* Na verdade, o que o ray-traicing faz é pegar somente os raios que partem, por exemplo, do seu personagem. Ele, basicamente, faz o caminho inverso da luz. Naturalmente, a luz emite os raios, que batem em um objeto e são refletidos para seu olho (câmera). Aqui, ***simulamos o raio de luz que parte da câmera, vai até o objeto e chega, por fim, na fonte de luz***. Assim, define-se quais são os raios necessários para se usar naquele momento para ***cada pixel***.

## Visão geral
O projeto será dividido em ***seis entregas,*** que consistem em: ***implementar um ray-tracer básico, interseção com malhas de triângulos, transformações afins, modelo de iluminação de phong, ray-tracer recursivo e por fim a(s) feature(s) extra(s)***; Cada entrega será corrigida individualmente e a avaliação também será individual. Na pagina de cada entrega é possível ver mais detalhes a respeito.

Tenha em mente que toda vez que nos referimos a ***pontos ou vetores*** nessa especificação, nos referimos a ***duas estruturas de dados compostas por 3 coordenadas (x, y, z), podendo ser floats ou doubles (recomendamos double para evitar erros de precisão e arredondamento de casas decimais).*** Tente usar apenas um dos dois tipos por toda a sua aplicação (floats ou double). Além disso, a ***cor RGB*** é formada por 3 parâmetros, um para o vermelho (R), um para o verde (G) e um para o azul (B), ***você pode armazenar essa cor em um vetor com 3 coordenadas***; porém, 0 ⩽ R, G, B ⩽ 255, podendo ser *floats* ou *doubles* (de novo só pra reforçar). ***Criem um repositório no github para gerenciar o projeto e facilitar a nossa correção.***

***OBS:***  é extremamente recomendado que vocês usem C++. Todo material de suporte é baseado em C++ e a linguagem oferece *operator overloading*, que se mostrará muito útil para trabalhar com pontos, vetores e transformações afins. (Apesar disso, não é obrigatório, o verdadeiro ray-tracing são os amigos que fazemos no caminho).

## Links importantes
- Especificação (Notion): https://short-crustacean-7e9.notion.site/71c2663a94304d629d33a8f289a7017e?v=96ba08a6ebe647e3aa5339ea8e019f3b

## Sobre o Octree
- Octree é uma estrutura de dados hierárquica que divide uma região 3D em sub-volumes menores. Cada cubo grande tem até 8 filhos (octantes) e tudo isso pode ser representado por uma tree. 
- A divisão pode ocorrer de forma recursiva

*Benefícios*
1. Eficiência na intersecção
2. Hierarquia no espaço
3. Divisão recursiva

> O maior benefício, no entanto, é reduzir o número de iterações feitas e checks pra saber se um objeto é atingido por um raio ou não. Com Octree, você só testa os raios que atravessam o objeto em determinado cubo (sub-volume)

*Construção da Octree:*
1. A Octree é construída a partir da cena, começando com um nó raiz que engloba toda a cena.
2. Se o número de objetos em um nó excede um limite, o nó é subdividido em oito filhos, cada um representando uma parte do espaço do nó pai.
3. Objetos são distribuídos nos filhos de acordo com sua posição.

*Interseção com Raio:*
1. Para determinar a interseção de um raio com objetos na cena, começamos pelo nó raiz.
2. Verificamos quais filhos do nó raiz são atravessados pelo raio e repetimos o processo recursivamente para esses filhos.
4. Testamos a interseção do raio apenas com objetos nos nós folhas que o raio atravessa.