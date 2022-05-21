
[x,Fe]=audioread('la.wav'); %lecture du signal
N=length(x);
f=(0:N-1)*Fe/N;  %axe des abcisses
Te=1/Fe;
t=(0:(N-1))*Te;
t1=linspace(0,1,N);
figure (1); hold on;
plot(t1,x, '-r');
myfontsize=14;
xlabel('temps','fontname','helvetica','fontsize', myfontsize,'fontweight','bold')
set(gca ,'xlim', [0,Fe], 'fontname','helvetica','fontsize', myfontsize-2 ,'fontweight','bold','linewidth',1)
ylabel('amplitude', 'fontname', 'helvetica', 'fontsize', myfontsize, 'fontweight', 'bold')
title('Signal temporel du fichier la.wav', 'fontname', 'helvetica', 'fontsize', 16, 'fontweight', 'bold')
hold off

figure (2);
%tracage de la fft de la.wav
X=fft(x);
modX=abs(X);
plot(f,modX, '-b');
myfontsize=14;
xlabel('fréquence','fontname','helvetica','fontsize', myfontsize,'fontweight','bold')
set(gca ,'xlim', [0,Fe], 'fontname','helvetica','fontsize', myfontsize-2 ,'fontweight','bold','linewidth',1)
ylabel('intensité (module de X)', 'fontname', 'helvetica', 'fontsize', myfontsize, 'fontweight', 'bold')
title('Spectre de la transformée discrète du signal', 'fontname', 'helvetica', 'fontsize', 16, 'fontweight', 'bold')
hold off
sound(x,Fe);




