program dgemm
implicit none
   integer :: n 
   real :: flops
   real :: mflops
   real(8), dimension(:,:), allocatable :: x
   real(8), dimension(:,:), allocatable :: y
   real(8), dimension(:,:), allocatable :: z
   real T1,T2
    
    integer :: i,j,k,run
    
do run=1000,2000,200  
    T1 = 0.0
    T2 = 0.0
    n = run
    flops = real(n)**3
    print *,"Matrix Multiplication for ", n,"*",n
    allocate(x(n,n))
    allocate(y(n,n))
    allocate(z(n,n))

    do i=1,n
       do j=1,n
          x(i,j) = sin(i*1D0)*sin(j*1D0)
          y(i,j) = cos(i*1D0)*cos(j*1D0)
          z(i,j) = 0
       enddo
    enddo

    CALL CPU_TIME(T1)
    !$acc kernels copyin(x(1:n,1:n),y(1:n,1:n),z(1:n,1:n)) 
    do i=1,n
       do j=1,n
          z(i,j) = 0
          do k=1,n
             z(i,j) = z(i,j) + x(i,k)*y(k,j)
          enddo
       enddo
    enddo
    !$acc end kernels
    CALL CPU_TIME(T2)
    mflops = abs(flops/10**6)
    print *,"Time (sec):", T2-T1
    print *,"Total Flops Calculated: ", flops
    print *,"Mflops/sec: ", mflops/abs(T2-T1)
    print *,"---------------------------------------------"
    deallocate(x)
    deallocate(y)
    deallocate(z)
enddo
end program dgemm
