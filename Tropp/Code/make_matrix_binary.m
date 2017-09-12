% make a matrix with rapdily decaying singular values and save to disk in binary format 
more off;

m = 1000;
n = 2000;
bin_file = '../matrix_data/A_mat_1kx2k.bin'
mat_file = '../matrix_data/A_mat_1kx2k.mat'
 

fprintf('making matrix..\n');
p = min(m,n);
if m >= n
   [U, temp] = qr(randn(m,n),0);
   [V, temp] = qr(randn(n));
else
   [U, temp] = qr(randn(m));
   [V, temp] = qr(randn(n,m),0);
end
if p>1
    S = logspace(1,-3,p);
else
    S = [1];
end
S = diag(S);
A = U*S*V';

M = A; 

% write to files
fprintf('write matrix M\n');
num_nnz = m*n;
fp = fopen(bin_file,'w');
fwrite(fp,m,'int32');
fwrite(fp,n,'int32');
for i=1:m
    fprintf('writing row %d of %d\n', i,m);
    for j=1:n
        fwrite(fp,M(i,j),'double');
    end
end
fclose(fp);
fprintf('writing to mat file too..\n');

save(mat_file,'M');

