from django.shortcuts import render, redirect, get_object_or_404
from .models import Category, Page
from .forms import CategoryForm, PageForm

def index(request):
    categories = Category.objects.all()

    # Handle category form submission directly on homepage
    if request.method == 'POST':
        form = CategoryForm(request.POST)
        if form.is_valid():
            form.save()
            return redirect('index')
    else:
        form = CategoryForm()

    return render(request, 'index.html', {'categories': categories, 'form': form})

def category_detail(request, category_id):
    category = get_object_or_404(Category, id=category_id)

    # increment visits only for GET requests
    if request.method == 'GET':
        category.visits += 1
        category.save()

    pages = Page.objects.filter(category=category)

    if request.method == 'POST':
        form = PageForm(request.POST)
        if form.is_valid():
            page = form.save(commit=False)
            page.category = category
            page.save()
            return redirect('category_detail', category_id=category.id)
    else:
        form = PageForm()

    return render(request, 'category_detail.html', {
        'category': category,
        'pages': pages,
        'form': form
    })

def like_category(request, category_id):
    category = get_object_or_404(Category, id=category_id)
    category.likes += 1
    category.save()
    return redirect('category_detail', category_id=category.id)

def view_page(request, page_id):
    page = get_object_or_404(Page, id=page_id)
    page.views += 1
    page.save()
    return redirect(page.url)
